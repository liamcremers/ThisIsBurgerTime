#pragma once
#include <list>
#include <string>

namespace dae
{
    class Observer;

    class Subject
    {
    public:
        virtual ~Subject();
        void AddObserver(Observer* observer);
        void RemoveObserver(Observer* observer);

        void Notify(const std::string& eventId);

    protected:
    private:
        std::list<Observer*> m_ObserverList;
    };

    class Observer
    {
        friend class Subject;

    protected:
        virtual void OnNotify(const std::string& eventId) = 0;
        virtual void OnDestroy() = 0;
    };
}
