#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>

#include "logging/DefaultLogFacility.h"
#include "model/DefaultModelController.h"
#include "model/task_manager/TaskManager.h"
#include "persistence/FilePersistence.h"
#include "service/DefaultTaskService.h"

int main(int argc, char** argv)
{
    using namespace task_manager;
    namespace po = boost::program_options;

    std::string log_level_str;
    po::options_description desc("Allowed options");
    desc.add_options()(
        "level,l", po::value<std::string>(&log_level_str)->default_value("info"),
        "Level of logging");

    po::variables_map opts;
    po::store(po::parse_command_line(argc, argv, desc), opts);
    po::notify(opts);

    auto log_level = logging::ConvertStringToLogLevel(log_level_str);

    if (log_level)
    {
        {
            auto format = "[%TimeStamp%][%Severity%](%Scope%): %Message%";
            logging::SetUp();
            logging::CreateFileLog("server", format, *log_level);
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

    auto id_producer = std::make_unique<TaskIdProducer>();
    auto task_manager = std::make_unique<TaskManager>(std::move(id_producer));

    auto file_persister = std::make_unique<FilePersistence>(
        "very_long_name_that_wont_conflict.txt");

    auto model_controller = std::make_unique<DefaultModelController>(
        std::move(task_manager), std::move(file_persister));

    auto server_address = "0.0.0.0:50051";

    BOOST_LOG_SEV(logger, logging::severinity::info)
        << "Server up on: " << server_address;
    DefaultTaskService service(std::move(model_controller));

    grpc::ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    server->Wait();
    BOOST_LOG_SEV(logger, logging::severinity::info) << "Server shutdown";

    return 0;
}
