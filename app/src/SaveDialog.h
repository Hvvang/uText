//
// Created by Artem Shemidko on 09.12.2020.
//

#ifndef UTEXT_SAVEDIALOG_H
#define UTEXT_SAVEDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class SaveDialog; }
QT_END_NAMESPACE

class SaveDialog : public QDialog {
Q_OBJECT
public:
    explicit SaveDialog(const QString &label = "", QWidget *parent = nullptr);
    ~SaveDialog() override;

    [[nodiscard]] const int &getResult() const;

    void accept() override;
    void reject() override;

private:
    int result{0};
    Ui::SaveDialog *ui;
};


#endif //UTEXT_SAVEDIALOG_H
