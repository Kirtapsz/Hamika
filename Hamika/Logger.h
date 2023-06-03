#pragma once

#include <KIR/hash/KIR5_sha512.h>

#include "Tools.h"
#include "Types.h"
#include "LoopControllInterface.h"
#include "Resource.h"

#include "Account.h"
#include "BluePrint.h"
#include "versions.h"

namespace Res
{
	namespace Log
	{
		struct Base
		{
			virtual ~Base();
		};

		struct RNGenerator;
		struct StandardKeyboard;
		struct ObjectValidator;

		struct Container: Record <
			TerminatedStringRecord<64>,
			TerminatedStringRecord<1024>,
			KIR5::sha512digest,
			KIR5::sha512digest,
			SwitchVectorRecord<Base, std::uint32_t, RNGenerator, StandardKeyboard, ObjectValidator>>
		{
			static constexpr std::size_t commit = 0;
			static constexpr std::size_t comment = 2;
			static constexpr std::size_t blueprintHash = 2;
			static constexpr std::size_t userHash = 3;
			static constexpr std::size_t records = 4;

			constexpr static std::array<const char *, 5> keys{{"Commit", "Comment", "BlueprintHash", "UserHash", "Records"}};
		};
	}

	struct Logger: public Base
	{
		std::string _uuid = uuid();
		const LoopControllerInterface &loopController;
		mutable std::shared_ptr<BluePrint> bluePrint;
		mutable std::shared_ptr<Account> account;
		Log::Container logContainer;

		inline Logger(LoopControllerInterface &loopController, const std::shared_ptr<BluePrint> &bluePrint, const std::shared_ptr<Account> &account):
			loopController(loopController), bluePrint(bluePrint), account(account)
		{

			std::get<logContainer.commit>(logContainer) = Versions::ApplicationCommitID;
			if (bluePrint)
			{
				std::get<logContainer.blueprintHash>(logContainer) = bluePrint->hash;
			}
			if (account)
			{
				std::get<logContainer.userHash>(logContainer) = account->hash();
			}
		}

		void operator=(const Log::Container &record);
		operator Log::Container() const;

		template<typename L, typename... _Types>
		void record(_Types... _Args)
		{
			auto &records = std::get<Log::Container::records>(logContainer);

			Log::Base *log = new L(
				static_cast<unsigned __int32>(loopController.loopCounter),
				static_cast<unsigned __int32>(loopController.currentTimeMS),
				std::forward<_Types>(_Args)...);

			records.push_back(std::shared_ptr<Log::Base>(log));
		}

		template<typename T>
		std::vector<T *> getLogs()
		{
			std::vector<T *> rows;

			auto &records = std::get<Log::Container::records>(logContainer);

			for (auto &it : records)
			{
				T *t = dynamic_cast<T *>(it.get());
				if (t != nullptr)
				{
					rows.push_back(t);
				}
			}

			std::sort(rows.begin(), rows.end(), [](const T *left, T *right)->bool
			{
				return std::get<T::loopCounter>(*left) < std::get<T::loopCounter>(*right);
			});

			return rows;
		}

		const static std::tuple<
			Handler<Log::Container>
		>
			handlers;

		void load(const std::string &filename);
		void save(const std::string &filename) const;
	};
}
