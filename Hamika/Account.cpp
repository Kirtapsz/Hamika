#pragma once

#include "Account.h"

namespace Res
{
	void Account::CompletedBluePrint::operator=(const Iv1Record &record)
	{
		_hash = std::get<Iv1Record::hash>(record);
		_date = std::get<Iv1Record::date>(record);
		_timeMS = std::get<Iv1Record::timeMS>(record);
	}
	Account::CompletedBluePrint::operator Iv1Record() const
	{
		Iv1Record record;
		std::get<Iv1Record::hash>(record) = _hash;
		std::get<Iv1Record::date>(record) = _date;
		std::get<Iv1Record::timeMS>(record) = _timeMS;
		return record;
	}

	KIR4::time Account::CompletedBluePrint::date() const
	{
		return _date;
	}
	std::uint32_t Account::CompletedBluePrint::timeMS() const
	{
		return _timeMS;
	}
	const KIR5::sha512digest &Account::CompletedBluePrint::hash() const
	{
		return _hash;
	}


	void Account::operator=(const Iv1Record &record)
	{
		_username = std::get<Iv1Record::username>(record);
		_hash = std::get<Iv1Record::hash>(record);
		_totalTimePlayedMS = std::get<Iv1Record::totalTimePlayedMS>(record);

		auto &t_completedBlueprint = std::get<Iv1Record::completedBlueprints>(record);
		_completedBlueprints.resize(t_completedBlueprint.size());
		for (std::size_t i = 0; i < _completedBlueprints.size(); ++i)
		{
			_completedBlueprints[i] = t_completedBlueprint[i];
		}
	}

	Account::operator Iv1Record() const
	{
		Iv1Record record;
		std::get<Iv1Record::username>(record) = _username;
		std::get<Iv1Record::hash>(record) = _hash;
		std::get<Iv1Record::totalTimePlayedMS>(record) = _totalTimePlayedMS;

		auto &t_completedBlueprint = std::get<Iv1Record::completedBlueprints>(record);
		t_completedBlueprint.resize(_completedBlueprints.size());
		for (std::size_t i = 0; i < _completedBlueprints.size(); ++i)
		{
			t_completedBlueprint[i] = _completedBlueprints[i];
		}
		return record;
	}

	const std::string &Account::username() const
	{
		return _username;
	}
	const KIR5::sha512digest &Account::hash() const
	{
		return _hash;
	}
	const std::uint32_t &Account::totalTimePlayedMS() const
	{
		return _totalTimePlayedMS;
	}
	const std::vector<Account::CompletedBluePrint> &Account::completedBlueprints() const
	{
		return _completedBlueprints;
	}

	void Account::addPlayedTime(std::uint32_t timeMS_)
	{
		_totalTimePlayedMS += timeMS_;
	}

	void Account::addBluePrint(const std::shared_ptr<BluePrint> &bluePrint_, std::uint32_t timeMS_)
	{
		auto item = std::find_if(_completedBlueprints.begin(), _completedBlueprints.end(), [&bluePrint_](const CompletedBluePrint &completedBluePrint) -> bool
		{
			return bluePrint_->hash == completedBluePrint._hash;
		});
		if (item != _completedBlueprints.end())
		{
			if (item->_timeMS > timeMS_)
			{
				item->_date = KIR4::time();
				item->_timeMS = timeMS_;
			}
		}
		else
		{
			CompletedBluePrint completedBluePrint;
			completedBluePrint._hash = bluePrint_->hash;
			completedBluePrint._date = KIR4::time();
			completedBluePrint._timeMS = timeMS_;

			_completedBlueprints.push_back(completedBluePrint);
		}
	}
	std::uint32_t Account::playTimeOn(const std::shared_ptr<BluePrint> &bluePrint_) const
	{
		auto item = std::find_if(_completedBlueprints.begin(), _completedBlueprints.end(), [&bluePrint_](const CompletedBluePrint &completedBluePrint) -> bool
		{
			return bluePrint_->hash == completedBluePrint._hash;
		});
		if (item != _completedBlueprints.end())
		{
			return item->timeMS();
		}
		return std::numeric_limits<std::uint32_t>::max();
	}


	void Accounts::operator=(const Iv1Record &record)
	{
		list.clear();
		for (auto &it : std::get<Iv1Record::account>(record))
		{
			std::shared_ptr<Account> account(new Account);
			*account = it;
			list.push_back(account);
		}
	}

	Accounts::operator Iv1Record() const
	{
		Iv1Record record;
		for (auto &it : list)
		{
			std::get<Iv1Record::account>(record).push_back(*it);
		}
		return record;
	}

	Accounts::ERR_C Accounts::add(const std::shared_ptr<Account> &account)
	{
		for (auto &it : list)
		{
			if (it->_username == account->_username)
			{
				return USERNAME_TAKEN;
			}
		}

		list.push_back(account);

		std::sort(list.begin(), list.end(), [](
			const std::shared_ptr<Account> &accountL,
			const std::shared_ptr<Account> &accountR)
		{
			return accountL->_username < accountR->_username;
		});

		return E_OK;
	}
	Accounts::ERR_C Accounts::add(const std::string &username, const std::string &password)
	{
		std::shared_ptr<Account> account(new Account);
		account->_username = username;

		if (password.length())
		{
			KIR5::sha512(account->_hash, reinterpret_cast<const unsigned char *>(password.data()), password.size());
		}
		else
		{
			memset(account->_hash.data(), 0, KIR5::SHA512_DIGEST_SIZE);
		}

		return add(account);
	}
	std::shared_ptr<Account> Accounts::get(const std::string &username)
	{
		for (auto &it : list)
		{
			if (it->_username == username)
			{
				return it;
			}
		}
		return nullptr;
	}

	const std::tuple<
		Handler<Accounts::Iv1Record>
	>
		Accounts::handlers{
			{true, nullptr, {0x48, 0x61, 0x6D, 0x53, 0x74, 0x30, 0x30, 0x31}, {0x48, 0x61, 0x6D, 0x53, 0x74, 0x30, 0x30, 0x31}, Hash::sha512_salty1, Crypto::V1_1024},
	};



	bool Accounts::initialize(std::uint32_t mode)
	{
		if (mode & RESET)
		{
			return true;
		}
		return LoadResource(*this);
	}
	void Accounts::shutdown()
	{
		SaveResource(*this);
	}

	Accounts accounts{"Hamika\\accounts.dat", Base::FILE_MISSING_ALLOWED};
}