#include "Observer.h"

void Subject::AddObserver(Observer* observer)
{
    m_ObserverList.push_back(observer);
}

void Subject::RemoveObserver(Observer* observer)
{
    m_ObserverList.remove(observer);
}

void Subject::Notify(const std::string& eventId)
{
    for (auto* observer : m_ObserverList)
    {
        observer->OnNotify(eventId);
    }
}
