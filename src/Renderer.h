//
// Created by leogout on 15/12/18.
//

#ifndef MULTISCALE_TURING_PATTERN_CPP_RENDERER_H
#define MULTISCALE_TURING_PATTERN_CPP_RENDERER_H


#include <QtCore/QArgument>
#include <QtCore/QString>

class Renderer : public QObject {
    Q_OBJECT
public:
    explicit Renderer();

public slots:
    void doWork(const QString &parameter) {
      QString result;
      /* ... here is the expensive or blocking operation ... */
      emit resultReady(result);
    }

    signals:
        void resultReady(const QString &result);
};

#endif //MULTISCALE_TURING_PATTERN_CPP_RENDERER_H
