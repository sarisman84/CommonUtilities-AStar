#pragma once
#include "Transform2D.h"
#include <vector>
#include <functional>
#include <tga2d/engine.h>


template<class Function>
struct Action
{
public:
	Action() = default;
	Action(std::function<Function()> aFunction)
	{
		myCallbacks.push_back(aFunction);
	}

	template<typename... InvokeArgs>
	inline void Invoke(InvokeArgs&&... someArgs);



	inline void operator +=(std::function<Function()> aFunction)
	{
		myCallbacks.push_back(aFunction);
	}
	inline void operator -=(std::function<Function()> aFunction)
	{
		myCallbacks.erase(std::remove(myCallbacks.begin(), myCallbacks.end(), aFunction), myCallbacks.end());
	}

	inline void operator=(Action<Function> someOther) = delete;

	inline bool operator==(Action<Function> someOther);
	inline bool operator!=(Action<Function> someOther);

private:
	//Function = void(*)
	std::vector<std::function<Function()>> myCallbacks;
};


class BoxCollider
{
public:
	BoxCollider() = default;
	BoxCollider(const Transform2D& aTransform)
	{
		myTransform = aTransform;
	}


	BoxCollider(const BoxCollider& aBoxCollider)
	{
		myTransform = aBoxCollider.myTransform;
	}

	void operator =(const BoxCollider& aBoxCollider)
	{
		myTransform = aBoxCollider.myTransform;
	}

	inline Action <void>& OnCollisionEnter() { return myOnEnterEvent; }
	inline Action <void>& OnCollisionExit() { return myOnExitEvent; }
	inline Action <void>& OnCollisionClick() { return myOnClickEvent; }

	bool CheckCollision(Transform2D aPoint)
	{
		auto ratio = Tga2D::Engine::GetInstance()->GetWindowRatio();

		bool xCheck = aPoint.myPosition.x  < myTransform.myPosition.x + (myTransform.mySize.x / 2.f) / ratio   && aPoint.myPosition.x > myTransform.myPosition.x - (myTransform.mySize.x / 2.f) / ratio;
		bool yCheck = aPoint.myPosition.y  < myTransform.myPosition.y + (myTransform.mySize.y / 2.f)  && aPoint.myPosition.y > myTransform.myPosition.y - (myTransform.mySize.y / 2.f);

		return xCheck && yCheck;
	}

	inline Transform2D& Transform() { return myTransform; }
	inline bool& HasCollidedAlready() { return myHasCollidedBefore; }



private:

	bool myHasCollidedBefore;
	Transform2D myTransform;
	Action<void> myOnEnterEvent;
	Action<void> myOnExitEvent;
	Action<void> myOnClickEvent;
};






template<class Function>
inline bool Action<Function>::operator==(Action<Function> someOther)
{

	return myCallbacks == someOther.myCallbacks;
}

template<class Function>
inline bool Action<Function>::operator!=(Action<Function> someOther)
{
	return myCallbacks != someOther.myCallbacks;
}

template<class Function>
template<typename ...InvokeArgs>
inline void Action<Function>::Invoke(InvokeArgs && ...someArgs)
{
	for (size_t i = 0; i < myCallbacks.size(); i++)
	{
		myCallbacks[i](someArgs...);
	}
}
