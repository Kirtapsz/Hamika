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
			class Func: public virtual Object::SceneInterface,
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
			class Func: public virtual Object::SceneInterface,
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

	class KeyboardPanel: public AdjusterPanel<Panel, 96, 96>
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

		public: KeyboardPanel();
		public: void updateKeyboardState(const std::shared_ptr<KeyboardController> &_keyboard_controller);
	};
	class LayerPanel: public AdjusterPanel<Panel, 32 * SceneLayer::number_of_layers, 32>
	{
		private: class Icon: public virtual AdjusterPanel<KIR5::FramedRectangleButton<>, 32, 32>, public virtual AdjustParam
		{
			friend class InteractiveMultitest;

			private: SceneDrawer_T<SceneBlock<Object::Brick>> *drawner_;
			private: std::size_t id_;
			private: bool is_active_ = false;
			private: KIR5::Button<>::Group button_group_;
			private: KIR5::Shared<AdjustablePanel<BmpButton, 2, 2, 32 - 4, 32 - 4>> bmp_button_;

			public: Icon(std::size_t _id, SceneDrawer_T<SceneBlock<Object::Brick>> *_drawner):
				AdjustParam(_id * 32, 0, 32, 32), drawner_(_drawner), id_(_id)
			{
				*this << bmp_button_;
				bmp_button_->show();
				bmp_button_->setBitmap(Res::uielements[Res::UIElements::Layers[_id]]);

				button_group_.fncPress = FNC_PRESS([&](FNC_PRESS_PARAMS)->FNC_PRESS_RET
				{
					setActive(!is_active_);
					drawner_->enableLayer(id_, is_active_);
				});
				button_group_.group(*this);
				button_group_.group(*bmp_button_);

				fncMoved.push_back(FNC_MOVED([&](FNC_MOVED_PARAMS)->FNC_MOVED_RET
				{
					setFrameSize(std::ceil(width() / 16.f));
				}));
				setColor(KIR5::Color(30, 30, 30));

				setActive(drawner_->isLayerEnabled(_id));
			}

			public: void setActive(bool _is_active)
			{
				is_active_ = _is_active;
				if (is_active_)
				{
					setColorFrame(KIR5::Color(30, 200, 30));
				}
				else
				{
					setColorFrame(KIR5::Color(200, 30, 30));
				}
			}
		};
		private:
		template<std::size_t I>
		constexpr std::shared_ptr<Icon> createIcon(SceneDrawer_T<SceneBlock<Object::Brick>> *_drawner)
		{
			return KIR5::Shared<Icon>(I, _drawner);
		}

		private:
		template<std::size_t... Is>
		constexpr std::array<std::shared_ptr<Icon>, sizeof...(Is)> createlayerStat(SceneDrawer_T<SceneBlock<Object::Brick>> *_drawner, std::index_sequence<Is...>)
		{
			return {createIcon<Is>(_drawner)...};
		}

		public: std::array<std::shared_ptr<Icon>, SceneLayer::number_of_layers> layer_states;

		public: LayerPanel(SceneDrawer_T<SceneBlock<Object::Brick>> *_drawner):
			layer_states(createlayerStat(_drawner, std::make_index_sequence<SceneLayer::number_of_layers>{}))
		{
			for (auto &icon : layer_states)
			{
				*this << icon;
			}
		}
	};
	class LoopCounter: public Label<Res::Consolas>
	{
		public: LoopCounter();
		public: void updateLoopcounter(unsigned long long _loop_counter);
	};

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
		private: KIR5::Event::FNC_TIMER coreTimer;
		private: std::uint8_t delayAlpha;

		private: std::shared_ptr<Res::Logger> logger;
		protected: std::shared_ptr<ValidatorRecord> validator;

		private: KIR5::Shared<KeyboardPanel> keyboard_Panel;
		private: std::shared_ptr<LayerPanel> layer_Panel;
		private: KIR5::Shared<LoopCounter> loopcounter_Label;

		public: Developer(const std::shared_ptr<Res::BluePrint> &bluePrint_);

		protected: void signGame();
		protected: void finishGame();

		public: virtual void failure(const std::string &msg);
	};

	class InteractiveMultitest: public Base::Standard::Type
	{
		private: KIR5::Shared<KeyboardPanel> keyboard_Panel;
		private: KIR5::Shared<LoopCounter> loopcounter_Label;

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