#pragma once
#include <list>
#include <string>
class Observer;

class Subject
{
public:
    virtual ~Subject();
    void AddObserver(Observer* observer);
    void RemoveObserver(Observer* observer);

protected:
    void Notify(const std::string& eventId);

private:
    std::list<Observer*> m_ObserverList{};
};

class Observer
{
    friend class Subject;

protected:
    virtual void OnNotify(const std::string& eventId) = 0;
    virtual void OnDestroy() = 0;
};
