#include "Observer.h"
#include <cassert>

Subject::~Subject() { Notify("SubjectDestroyed"); }

void Subject::AddObserver(Observer* observer)
{
    assert(observer && "Observer must not be null");
    m_ObserverList.push_back(observer);
}

void Subject::RemoveObserver(Observer* observer)
{
    m_ObserverList.remove(observer);
}

void Subject::Notify(const std::string& eventId)
{
    for (const auto& observer : m_ObserverList)
    {
        observer->OnNotify(eventId);
    }
}
