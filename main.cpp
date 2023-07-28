#include <QCoreApplication>
#include <QProcess>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString repoPath = "C:/Users/11/Documents/GMThree"; // 仓库路径

    QProcess gitProcess;
    gitProcess.setWorkingDirectory(repoPath);
    gitProcess.start("git", QStringList() << "log" << "-1"); // 获取最近一次提交信息
    gitProcess.waitForFinished(-1);

    QString output = gitProcess.readAllStandardOutput();
    QString error = gitProcess.readAllStandardError();

    if (error.isEmpty()) {
        qDebug() << "最近一次提交信息：" << output;
    } else {
        qDebug() << "获取提交信息出错：" << error;
    }

    return a.exec();
}
