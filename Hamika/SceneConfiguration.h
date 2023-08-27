#pragma once

#include "SceneAction.h"
#include "SceneArt.h"
#include "SceneControl.h"
#include "SceneField.h"
#include "SceneMurphy.h"
#include "Font.h"

#pragma warning(disable: 4250)

namespace UI::Scene
{
	namespace Base
	{
		namespace Standard
		{
			class Data:
				public virtual Module::Field::Data,
				public virtual Module::Murphy::Data,
				public virtual Module::Art::Data,
				public virtual Module::Action::Data,
				public virtual Module::Contol::Data
			{
				protected: bool finished = false;
			};

			template <typename DATA>
			class Func: public virtual Object::Brick::Interface,
				public virtual Module::Field::Func<DATA>,
				public virtual Module::Murphy::Func<DATA>,
				public virtual Module::Art::Func<DATA>,
				public virtual Module::Action::Func<DATA>,
				public virtual Module::Contol::Func<DATA>
			{
				protected: void initialize(const std::shared_ptr<Res::BluePrint> &bluePrint_,
										   const std::shared_ptr<Res::Account> &_account)
				{
					loopStart();
					Module::Field::Func<DATA>::initialize(bluePrint_);
					Module::Murphy::Func<DATA>::initialize(_account);
					Module::Art::Func<DATA>::initialize();
					Module::Action::Func<DATA>::initialize();
					Module::Contol::Func<DATA>::initialize();
				}
			};

			typedef Func<Data> Type;
		}
		namespace MultitestMock
		{
			class Data:
				public virtual Module::Field::Data,
				public virtual Module::Murphy::Data,
				public virtual Module::Art::Mock::Data,
				public virtual Module::Action::Data,
				public virtual Module::Contol::Mock::Data
			{
				protected: bool finished = false;
			};

			template <typename DATA>
			class Func: public virtual Object::Brick::Interface,
				public virtual Module::Field::Func<DATA>,
				public virtual Module::Murphy::Func<DATA>,
				public virtual Module::Art::Mock::Func<DATA>,
				public virtual Module::Action::Func<DATA>,
				public virtual Module::Contol::Mock::Func<DATA>,
				public virtual Validator::Interface
			{
				protected: void initialize(const std::shared_ptr<Res::BluePrint> &bluePrint_,
										   const std::shared_ptr<Res::Account> &_account)
				{
					loopStart();
					Module::Field::Func<DATA>::initialize(bluePrint_);
					Module::Murphy::Func<DATA>::initialize(_account);
					Module::Art::Mock::Func<DATA>::initialize();
					Module::Action::Func<DATA>::initialize();
					Module::Contol::Mock::Func<DATA>::initialize();
				}
			};

			typedef Func<Data> Type;
		}
	}

	constexpr std::size_t ACTIVE_MAP_FINISHED = 0x400;

	class Replay: public Base::Standard::Type
	{
		private: unsigned long long delayLoop;
		private: static constexpr unsigned long long startDelay = CPS * 0.5f;
		private: static constexpr unsigned long long stopDelay = CPS * 2;
		private: KIR5::Event::FNC_TIMER delayTimerStart;
		private: KIR5::Event::FNC_TIMER delayTimerStop;
		private: KIR5::Event::FNC_DRAW delayDraw;
		private: std::uint8_t delayAlpha;

		private: std::shared_ptr<Res::Logger> logger;

		public: Replay(const std::string &uuid_);

		protected: void signGame();
		protected: void finishGame();

		public: virtual void failure(const std::string &msg);
	};

	class Play: public Base::Standard::Type
	{
		private: unsigned long long delayLoop;
		private: static constexpr unsigned long long startDelay = CPS * 0.5f;
		private: static constexpr unsigned long long stopDelay = CPS * 2;
		private: KIR5::Event::FNC_TIMER delayTimerStart;
		private: KIR5::Event::FNC_TIMER delayTimerStop;
		private: KIR5::Event::FNC_DRAW delayDraw;
		private: std::uint8_t delayAlpha;

		private: std::shared_ptr<Res::Logger> logger;

		public: Play(const std::shared_ptr<Res::BluePrint> &bluePrint_, const std::shared_ptr<Res::Account> &account_);

		protected: void signGame();
		protected: void finishGame();

		public: virtual void failure(const std::string &msg);
	};

	class Developer: public Base::Standard::Type
	{
		private: bool paused = false;
		private: unsigned long long delayLoop;
		private: static constexpr unsigned long long startDelay = CPS * 0.5f;
		private: static constexpr unsigned long long stopDelay = CPS * 2;
		private: KIR5::Event::FNC_TIMER delayTimerStart;
		private: KIR5::Event::FNC_TIMER delayTimerStop;
		private: KIR5::Event::FNC_DRAW delayDraw;
		private: std::uint8_t delayAlpha;

		private: std::shared_ptr<Res::Logger> logger;
		protected: std::shared_ptr<ValidatorRecord> validator;

		public: Developer(const std::shared_ptr<Res::BluePrint> &bluePrint_);

		protected: void signGame();
		protected: void finishGame();

		public: virtual void failure(const std::string &msg);
	};

	class InteractiveMultitest: public Base::Standard::Type
	{
		private: class Icon: public virtual Panel
		{
			friend class InteractiveMultitest;

			private: bool is_active_ = false;
			private: KIR5::Bitmap bitmap_;

			public: Icon();

			public: void setActive(bool _is_active);
			public: void setBitmap(const KIR5::Bitmap &bitmap);
		};

		private: KIR5::Shared<AdjusterPanel<Panel, 96, 96>> keyboard_Panel;
		private: std::array<std::array<std::shared_ptr<Icon>, 3>, 3> keyboard_state{{
			{
				KIR5::Shared<AdjustablePanel<Icon, 0, 0, 32, 32>>(),
				KIR5::Shared<AdjustablePanel<Icon, 0, 32, 32, 32>>(),
				KIR5::Shared<AdjustablePanel<Icon, 0, 64, 32, 32>>(),
			},
			{
				KIR5::Shared<AdjustablePanel<Icon, 32, 0, 32, 32>>(),
				KIR5::Shared<AdjustablePanel<Icon, 32, 32, 32, 32>>(),
				KIR5::Shared<AdjustablePanel<Icon, 32, 64, 32, 32>>(),
			},
			{
				KIR5::Shared<AdjustablePanel<Icon, 64, 0, 32, 32>>(),
				KIR5::Shared<AdjustablePanel<Icon, 64, 32, 32, 32>>(),
				KIR5::Shared<AdjustablePanel<Icon, 64, 64, 32, 32>>(),
			},
		}};
		private: void updateKeyboardState();

		private: KIR5::Shared<Label<Res::Consolas>> loopcounter_Label;
		private: void updateLoopcounter();

		private: bool paused = false;
		private: KIR5::Event::FNC_TIMER coreTimer;
		private: std::int32_t deplayTimer;
		private: std::int8_t speed;
		private: std::shared_ptr<Res::Logger> logger;
		protected: std::shared_ptr<ValidatorReplay> validator;

		public: InteractiveMultitest(const std::string &filename, std::int8_t speed);

		protected: void signGame();
		protected: void finishGame();

		public: virtual void failure(const std::string &msg);
	};

	class Multitest: public Base::MultitestMock::Type
	{
		private: std::shared_ptr<Res::Logger> logger;
		protected: std::shared_ptr<ValidatorReplay> validator;

		public: Multitest(const std::string &filename);

		protected: void signGame();
		protected: void finishGame();

		public: virtual void failure(const std::string &msg);
	};
}

#pragma warning(default:4250)