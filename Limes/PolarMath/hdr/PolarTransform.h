#pragma once
#include <memory>
#include <forward_list>
#include <functional>
#include <system_error>

#include <algorithm>

#include "Constants.h"
#include "PolarPoint.h"
#include "Vector2D.h"
#include "Globals.h"


/*
	template<class T>
	class TDelegate;

	template<class return_type, class ...arg_types>
	class TDelegate<return_type(arg_types ...)>
	{
	public:
		using callback_type = typename return_type(*)(void *, arg_types ...);
		
		TDelegate(bool bIsMember, void *pCaller, callback_type Function) :
			m_pCaller{ pCaller },
			m_pCallback{ Function },
			m_bIsMember{ bIsMember }
		{
		}

		template<return_type(*FuncPtr)(arg_types ...)>
		static TDelegate Create()
		{
			return TDelegate(false, nullptr, &GlobalCaller<FuncPtr> );


		}

		template<class scope_type, return_type(scope_type::*pFunction)(arg_types ...)>
		static TDelegate Create(scope_type *pObj)
		{
			return TDelegate(true, pObj, &MemberCaller<scope_type, pFunction>);


		}
				   		 	  
		return_type operator()(arg_types ...Args)
		{
			return m_pCallback(m_pCaller, Args...);


		}

		bool operator==(const TDelegate &Rhs)
		{
			return m_pCaller == Rhs.m_pCaller && m_pCallback == Rhs.m_pCallback;


		}

		bool IsBound() const noexcept
		{
			if (m_bIsMember && m_pCaller == nullptr)
			{
				return false;

			}
			return true;


		}

	private:
		template<return_type(*pFunction)(arg_types ...)>
		static return_type GlobalCaller(void *, arg_types ...Args)
		{
			return pFunction(Args ...);


		}

		template<class scope_type, return_type(scope_type::*pFunction)(arg_types ...)>
		static return_type MemberCaller(void *pCaller, arg_types ...Args)
		{
			//fixup null ptr
			return (static_cast<scope_type *>(pCaller)->*pFunction)(Args ...);


		}

		void *m_pCaller;
		callback_type m_pCallback;
		bool m_bIsMember;
			   

	};

	template<class T>
	class TMultiDelegate;

	template<class return_type, class ... arg_types>
	class TMultiDelegate<return_type(arg_types ...)>
	{
	public:
		using delegate_type = TDelegate<return_type(arg_types ...)>;

		void Add(delegate_type &Delegate)
		{
			m_InvocationList.push_front(Delegate);


		}

		void Remove(delegate_type &Delegate)
		{
			m_InvocationList.remove(Delegate);


		}

		bool GetIsEmpty() const noexcept
		{
			return m_InvocationList.empty();


		}

		void Broadcast(arg_types ...Args)
		{
			for (auto &&Function : m_InvocationList)
			{
				if (Function.IsBound())
				{
					Function(Args ...);

				}


			}


		}

	protected:
		std::forward_list<delegate_type> m_InvocationList;



		
	};
*/

namespace PolarMath
{	
	struct FAngle
	{
		double m_Angle;

		FAngle(double Angle)
		{
			m_Angle = std::fmod(Angle, 360);


		}

		operator double() const
		{
			return m_Angle;


		}

		FAngle &operator=(double Rhs)
		{
			m_Angle = std::fmod(Rhs, 360);


		}

		FAngle &operator-=(const FAngle &Rhs)
		{
			this->m_Angle = std::fmod(this->m_Angle - Rhs.m_Angle, 360);
			if (this->m_Angle < 0)
			{
				this->m_Angle = 360 - std::abs(this->m_Angle);

			}

			return *this;


		}

		FAngle &operator+=(const FAngle &Rhs)
		{
			this->m_Angle = std::fmod(this->m_Angle + Rhs.m_Angle, 360);
			return *this;


		}

		bool operator==(const FAngle &Rhs)
		{
			return	Rhs.m_Angle < this->m_Angle + ReqPrecision
					&& Rhs.m_Angle > this->m_Angle - ReqPrecision;

		}

		bool operator==(double Angle)
		{
			auto Wrapped{ std::fmod(Angle, 360) };

			return	Wrapped < this->m_Angle + ReqPrecision
					&& Wrapped > this->m_Angle - ReqPrecision;

		}


	};

	/*
	FAngle operator+(FAngle Lhs, const FAngle &Rhs)
	{
		Lhs += Rhs;
		return Lhs;


	}

	FAngle operator-(FAngle Lhs, const FAngle &Rhs)
	{
		Lhs -= Rhs;
		return Lhs;


	}
	*/

	class CPolarTransform
	{
	public:
		CPolarTransform() = default;

		CPolarTransform(double X, double Y) :
			m_CartesianPos{ X, Y }
		{}

		SPolarPoint TransformPointTo(const CPolarTransform &Target, const SPolarPoint &Point) const;

		void TranformPointsTo(const CPolarTransform &Target, const SPolarPoint(&Points)[4], SPolarPoint (&OutPoints)[4]) const;

		class CPolarCollider TransformColliderTo(const CPolarTransform &Target, const CPolarCollider &Collider) const;

		bool operator==(const CPolarTransform Rhs) const;

	protected:
		SVector2D m_CartesianPos;


	};




}

