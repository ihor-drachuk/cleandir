#include <QCoreApplication>
#include <QDir>

#include <iostream>

enum StatusCodes
{
    OK = 0,
    NoDirs     = 1,
    CantFind   = 2,
    CantRemove = 4
};

bool clear(QDir& dir)
{
    bool ok = true;
    auto files = dir.entryList(QDir::Filter::Files);

    for (const auto& x : qAsConst(files))
        ok &= dir.remove(x);

    auto dirs = dir.entryList(QDir::Filter::AllDirs | QDir::Filter::NoDotAndDotDot);
    for (const auto& x : qAsConst(dirs))
        ok &= QDir(dir.absoluteFilePath(x)).removeRecursively();

    return ok;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    int status = StatusCodes::OK;

    auto dirs = a.arguments();
    dirs.removeFirst();

    if (dirs.isEmpty()) {
        std::cerr << "No directories provided!" << std::endl;
        return NoDirs;
    }

    for (const auto& x : qAsConst(dirs)) {
        QDir dir(x);

        if (!dir.exists()) {
            std::cerr << "Can't find directory: " << x.toStdString() << std::endl;
            status |= CantFind;
            continue;
        }

        auto ok = clear(dir);
        if (ok) {
            std::cout << "Cleared directory: " << x.toStdString() << std::endl;
        } else {
            std::cerr << "Failed to clear directory: " << x.toStdString() << std::endl;
            status |= CantRemove;
        }
    }

    return status;
}
