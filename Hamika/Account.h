#pragma once

#include "Resource.h"
#include "Tools.h"
#include "Hash.h"
#include "Crypto.h"
#include "BluePrint.h"

#include <KIR/KIR4_time.h>

namespace Res
{
	struct Account
	{
		friend class Accounts;

		public: struct Exception
		{
			struct UsernameTaken: public std::exception
			{
				const char *what() const throw ()
				{
					return "Username is already taken.";
				}
			};
		};


		public: struct CompletedBluePrint
		{
			friend struct Account;

			private: std::string _replayUuid;
			private: KIR5::sha512digest _bluePrintHash;
			private: std::uint64_t _date;
			private: std::uint32_t _timeMS;

			private: struct Iv1Record: KIR5::StreamRecords::Record<
				std::string,
				KIR5::sha512digest,
				std::uint64_t,
				std::uint32_t
				   >
			{
				constexpr static std::size_t replayUuid = 0;
				constexpr static std::size_t bluePrintHash = 1;
				constexpr static std::size_t date = 2;
				constexpr static std::size_t timeMS = 3;

				constexpr static std::array<const char *, 4> keys{{"ReplayUUID","BluePrintHash","CompletedDate","TimePlayedMS"}};
			};

			private: void operator=(const Iv1Record &record);
			private: operator Iv1Record() const;

			public: KIR4::time date() const;
			public: std::uint32_t timeMS() const;
			public: const std::string &replayUuid() const;
			public: const KIR5::sha512digest &bluePrintHash() const;

		};

		private: std::string _username;
		private: KIR5::sha512digest _hash;
		private: std::uint32_t _totalTimePlayedMS{0};
		private: std::vector<CompletedBluePrint> _completedBlueprints;

		private: struct Iv1Record: KIR5::StreamRecords::Record<
			std::string,
			KIR5::sha512digest,
			std::uint32_t,
			KIR5::StreamRecords::Vector<std::uint16_t, CompletedBluePrint::Iv1Record>
			   >
		{
			constexpr static std::size_t username = 0;
			constexpr static std::size_t hash = 1;
			constexpr static std::size_t totalTimePlayedMS = 2;
			constexpr static std::size_t completedBlueprints = 3;

			constexpr static std::array<const char *, 4> keys{{"Username","Hash","TotalTimePlayedMS","CompletedBlueprints"}};
		};

		private: void operator=(const Iv1Record &record);
		private: operator Iv1Record() const;

		public: const std::string &username() const;
		public: const std::uint32_t &totalTimePlayedMS() const;
		public: const std::vector<CompletedBluePrint> &completedBlueprints() const;
		public: const KIR5::sha512digest &hash() const;
		public: void addPlayedTime(std::uint32_t timeMS_);
		public: void addBluePrint(const std::shared_ptr<BluePrint> &bluePrint_, std::uint32_t timeMS_, const KIR4::time &date, const std::string &replayUuid);
		public: const CompletedBluePrint *getRecordOn(const std::shared_ptr<BluePrint> &bluePrint_) const;
	};

	class Accounts: public Base
	{
		using Base::Base;

		public: std::vector<std::shared_ptr<Account>> list;

		public: struct Iv1Record: KIR5::StreamRecords::Record<
			KIR5::StreamRecords::Vector<std::uint16_t, Account::Iv1Record>
			  >
		{
			constexpr static std::size_t account = 0;

			constexpr static std::array<const char *, 1> keys{{"Account"}};
		};

		public: void operator=(const Iv1Record &record);
		public: operator Iv1Record() const;

		public: void add(const std::shared_ptr<Account> &account);
		public: void add(const std::string &username, const std::string &password);
		public: std::shared_ptr<Account> get(const std::string &username);

		public: const static std::tuple<
			Handler<Accounts::Iv1Record>
			  >
			handlers;

		public: virtual bool initialize(std::uint32_t mode);
		public: virtual void shutdown();
	};

	extern Accounts accounts;
}