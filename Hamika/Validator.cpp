#include "Validator.h"


void ValidatorRecord::validate(std::uint8_t validateType_, const ::Json &objectProperties_)
{
	logger.record<Res::Log::ObjectValidator>(validateType_, objectProperties_);
}


ValidatorReplay::ValidatorReplay(LoopControllerInterface &loopControllerInterface, Res::Logger &logger, Interface &scene):
	Validator(loopControllerInterface, scene)
{
	rows = logger.getLogs<Res::Log::ObjectValidator>();
	it = rows.begin();
}


bool compareJson(LoopControllerInterface &loopControllerInterface, Json &object, const Json &validator)
{
	for (auto &[key, value] : validator.items())
	{
		if (object.contains(key))
		{
			if (object[key].is_number_float())
			{
				int left = static_cast<int>(object[key].get<float>() * 1000);
				int right = static_cast<int>(value.get<float>() * 1000);

				if (left != right)
				{
					loopControllerInterface.failure(concatenate("Value difference on: ", key, " : ", object[key], " instead of ", value));
					return false;
				}
			}
			else if (object[key].is_boolean() || object[key].is_number() || object[key].is_string())
			{
				if (object[key] != value)
				{
					loopControllerInterface.failure(concatenate("Value difference on: ", key, " : ", object[key], " instead of ", value));
					return false;
				}
			}
			else if (!compareJson(loopControllerInterface, object[key], value))
			{
				loopControllerInterface.failure(concatenate("Failed on \\", key));
				return false;
			}
		}
		else
		{
			loopControllerInterface.failure(concatenate("Key not found: ", key));
			return false;
		}

	}
	return true;
}

void ValidatorReplay::validate()
{
	while (it != rows.end())
	{
		auto &loopCounter = std::get<Res::Log::ObjectValidator::loopCounter>(**it);
		if (loopCounter < loopControllerInterface.loopCounter)
		{
			++it;
			loopControllerInterface.failure(KIR5::concatenate("ValidatorReplay loop was jumped at ", loopCounter, " (",
														std::get<Res::Log::ObjectValidator::validateType>(**it), ", ",
														std::get<Res::Log::ObjectValidator::objectProperties>(**it), ")"));
		}
		else if (loopCounter == loopControllerInterface.loopCounter)
		{
			auto oValidator = *it++;
			auto &oValidatorType = std::get<Res::Log::ObjectValidator::validateType>(*oValidator);
			auto &oValidatorProperties = std::get<Res::Log::ObjectValidator::objectProperties>(*oValidator);

			switch (oValidatorType)
			{
				case Res::Log::ObjectValidator::OBJECT_T:
				{
					if (!compareJson(loopControllerInterface, scene.GetObject({
						oValidatorProperties["\\ObjectCoordModule"]["coord.x"].get<Type::Coord::base>(),
						oValidatorProperties["\\ObjectCoordModule"]["coord.y"].get<Type::Coord::base>(),
																			  }).print(),
																			  oValidatorProperties))
					{
						loopControllerInterface.failure(KIR5::concatenate("ValidatorReplay compare failed on object ", loopCounter, " (",
																	oValidatorType, ", ",
																	oValidatorProperties, ")"));
					}
					break;
				}
				case Res::Log::ObjectValidator::REMAIN_T:
				{
					if (!compareJson(loopControllerInterface, scene.GetRemain({
						oValidatorProperties["\\ObjectCoordModule"]["coord.x"].get<Type::Coord::base>(),
						oValidatorProperties["\\ObjectCoordModule"]["coord.y"].get<Type::Coord::base>(),
																			  }).print(),
																			  oValidatorProperties))
					{
						loopControllerInterface.failure(KIR5::concatenate("ValidatorReplay compare failed on remain ", loopCounter, " (",
																	oValidatorType, ", ",
																	oValidatorProperties, ")"));
					}
					break;
				}
				case Res::Log::ObjectValidator::STATUSBAR_T:
				{
					if (!compareJson(loopControllerInterface, scene.printStatusBar(), oValidatorProperties))
					{
						loopControllerInterface.failure(KIR5::concatenate("ValidatorReplay compare failed on statusbar ", loopCounter, " (",
																	oValidatorType, ", ",
																	oValidatorProperties, ")"));
					}
					break;
				}
				case Res::Log::ObjectValidator::TERMINATE_T:
				{
					scene.terminateScene();
					break;
				}
				case Res::Log::ObjectValidator::RESULT_T:
				{
					if (!compareJson(loopControllerInterface, scene.printResult(), oValidatorProperties))
					{
						loopControllerInterface.failure(KIR5::concatenate("ValidatorReplay compare failed on result ", loopCounter, " (",
																	oValidatorType, ", ",
																	oValidatorProperties, ")"));
					}
					break;
				}
				default:
				{
					loopControllerInterface.failure(KIR5::concatenate("ValidatorReplay invalid type at ", loopCounter, " (",
																oValidatorType, ", ",
																oValidatorProperties, ")"));
				}
			}
		}
		else
		{
			break;
		}
	}
}

void ValidatorReplay::finalize()
{
	validate();
	if (it != rows.end())
	{
		loopControllerInterface.failure(concatenate("ValidatorReplay failed because some of the records were not processed at ", loopControllerInterface.loopCounter));
	}
	else
	{
		scene.setResult(Interface::RESULT::PASSED);
	}
}
