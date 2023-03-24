#ifndef REQUESTMERGER_H
#define REQUESTMERGER_H

#include <functional>
#include <memory>

#include <QObject>
#include <QTimer>

class RequestMerger : public QObject {
    Q_OBJECT

public:
    using ExecuteCallback = std::function<void()>;
    using ShouldActivateCallback = std::function<bool()>;
    using ShouldDeactivateCallback = std::function<bool()>;

public:
    explicit RequestMerger(int period);
    ~RequestMerger() override;

    void init(ExecuteCallback activationExecuteCallback,
              ExecuteCallback deactivationExecuteCallback,
              ShouldActivateCallback shouldAcitvateCallback,
              ShouldDeactivateCallback shouldDeactivateCallback);

    void activate();
    void deactivate();

private slots:
    void execute();

private:
    class State;
    class IdleState;
    class ActivationState;
    class DeactivationState;

private:
    void enterIdleState();
    void enterActivationState();
    void enterDeactivationState();

    void start();
    void stop();

    void updateCurrentState(std::shared_ptr<State> newState);

private:
    // 请求合并的最小周期是10ms
    const int LEAST_PERIOD = 10;
    int period_ = LEAST_PERIOD;
    QTimer timer_;

    std::shared_ptr<State> currentSate_ = nullptr;
    std::shared_ptr<State> idleState_ = nullptr;
    std::shared_ptr<State> activationState_ = nullptr;
    std::shared_ptr<State> deactivationState_ = nullptr;
};

#endif // REQUESTMERGER_H
