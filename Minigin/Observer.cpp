#include "Observer.h"
#include <cassert>

dae::Subject::~Subject()
{
    for (const auto& observer : m_ObserverList)
    {
        observer->OnDestroy();
    }
}

void dae::Subject::AddObserver(Observer* observer)
{
    assert(observer && "Observer must not be null");
    m_ObserverList.push_back(observer);
}

void dae::Subject::RemoveObserver(Observer* observer)
{
    m_ObserverList.remove(observer);
}

void dae::Subject::Notify(const std::string& eventId)
{
    for (const auto& observer : m_ObserverList)
    {
        observer->OnNotify(eventId);
    }
}
