#include <boost/program_options.hpp>

#include "client/TaskServiceModelController.h"
#include "interactor/io_facility/iostream/IostreamIoFacility.h"
#include "interactor/small_steps/default/DefaultSmallStepFactory.h"
#include "interactor/state_machine/StateMachineController.h"
#include "interactor/validator/DefaultValidator.h"
#include "logging/DefaultLogFacility.h"

int main(int argc, char** argv)
{
    using namespace task_manager;
    namespace po = boost::program_options;

    std::string log_level_str;
    std::string host;
    std::string port;
    po::options_description desc("Allowed options");
    desc.add_options()(
        "level,l", po::value<std::string>(&log_level_str)->default_value("info"),
        "Level of logging")(
        "host,h", po::value<std::string>(&host)->default_value("127.0.0.1"),
        "Host to connect to")(
        "port,p", po::value<std::string>(&port)->default_value("50051"),
        "Port to connect to");

    po::variables_map opts;
    po::store(po::parse_command_line(argc, argv, desc), opts);
    po::notify(opts);

    auto log_level = logging::ConvertStringToLogLevel(log_level_str);

    if (log_level)
    {
        {
            auto format = "[%TimeStamp%][%Severity%](%Scope%): %Message%";
            logging::SetUp();
            logging::CreateFileLog("client", format, *log_level);
        }
    }
    else
    {
        std::cerr << "Invalid level option.\n"
                     "  Allowed options: \n"
                     "   - 'trace'\n"
                     "   - 'debug'\n"
                     "   - 'info'\n"
                     "   - 'warning'\n"
                     "   - 'error'\n"
                     "   - 'fatal'\n";
        return 1;
    }

    auto& logger = logging::GetDefaultLogger();
    BOOST_LOG_SEV(logger, logging::severity::info)
        << "Connecting to " << host << ":" << port;

    auto validator = std::make_shared<DefaultValidator>();
    auto io_facility = std::make_shared<IostreamIoFacility>();
    auto small_step_factory =
        std::make_shared<DefaultSmallStepFactory>(io_facility, validator);

    auto state_machine = std::make_unique<StateMachine>(validator, io_facility,
                                                        small_step_factory);

    std::string server_address = host + ":" + port;
    auto stub = std::make_unique<TaskService::Stub>(
        grpc::CreateChannel(server_address, grpc::InsecureChannelCredentials()));
    auto task_service_model_controller =
        std::make_shared<TaskServiceModelController>(std::move(stub));
    StateMachineController ctrl{task_service_model_controller,
                                std::move(state_machine)};
    ctrl.Run();

    return 0;
}