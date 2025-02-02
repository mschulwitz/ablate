#include "fileLog.hpp"
#include <utilities/mpiError.hpp>
#include <utilities/petscError.hpp>
#include "environment/runEnvironment.hpp"

ablate::monitors::logs::FileLog::FileLog(std::string fileName)
    : outputPath(std::filesystem::path(fileName).is_absolute() ? std::filesystem::path(fileName) : ablate::environment::RunEnvironment::Get().GetOutputDirectory() / fileName), file(nullptr) {}

ablate::monitors::logs::FileLog::~FileLog() {
    if (file) {
        fclose(file);
    }
}

void ablate::monitors::logs::FileLog::Initialize(MPI_Comm commIn) {
    comm = commIn;
    PetscFOpen(comm, outputPath.c_str(), "w", &file) >> checkError;
}

void ablate::monitors::logs::FileLog::Printf(const char* format, ...) {
    if (file) {
        va_list args;
        va_start(args, format);
        PetscVFPrintf(file, format, args) >> checkError;
        va_end(args);
    }
}

#include "parser/registrar.hpp"
REGISTER(ablate::monitors::logs::Log, ablate::monitors::logs::FileLog, "Writes the result of the log to a file", ARG(std::string, "name", "the name of the log file"));
