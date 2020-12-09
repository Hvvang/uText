//
// Created by Artem Shemidko on 09.12.2020.
//

#ifndef UTEXT_EDITOR_H
#define UTEXT_EDITOR_H

#include <Qsci/qsciscintilla.h>

#include <QFile>

class Editor : public QsciScintilla {
Q_OBJECT
public:
    explicit Editor(QWidget* parent = nullptr);
    virtual ~Editor();

    void setFile(QFile *file);
    void setLex(const QString &fPath);
    [[nodiscard]] const bool &isEdited() const;
    void setEdited(const bool &edited);

public slots:
    [[maybe_unused]] void fold_by_level(int level = 0);

private:
    QFile *file{Q_NULLPTR};
    QsciLexer *lex{Q_NULLPTR};
    bool edited{false};


};


#endif //UTEXT_EDITOR_H
