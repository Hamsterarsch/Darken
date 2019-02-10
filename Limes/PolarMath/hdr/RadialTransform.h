#pragma once
#include <memory>
#include <forward_list>
#include <functional>
#include <system_error>

#include <algorithm>

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
	constexpr double PI = 3.14159265358979323846;
	constexpr double ToRadCoeff = PI / 180;
	constexpr double ToDegCoeff = 180 / PI;
	constexpr double ReqPrecision = 1e-12;
	
	double GetShortestAngleBetween(double FirstAngle, double SecondAngle)
	{
		auto WrappedAngle{ std::abs(SecondAngle - FirstAngle) };
		return WrappedAngle > 180 ? 360 - WrappedAngle : WrappedAngle;


	}

	double GetShortestAngleBetweenSigned(double FirstAngle, double SecondAngle)
	{
		auto SignedWrappedAngle{ SecondAngle - FirstAngle };

		auto UnsignedWrappedAngle{ std::abs(SignedWrappedAngle) };

		auto UnsignedUnwrappedAngle{ UnsignedWrappedAngle };
		if (UnsignedWrappedAngle > 180)
		{
			if (std::signbit(SignedWrappedAngle))
			{
				UnsignedUnwrappedAngle = 360 - UnsignedWrappedAngle;

			}
			else
			{
				UnsignedUnwrappedAngle = UnsignedWrappedAngle - 360;

			}
			return UnsignedUnwrappedAngle;

		}
		//auto UnsignedUnwrappedAngle{ UnsignedWrappedAngle > 180 ? 360 - UnsignedWrappedAngle : UnsignedWrappedAngle };

		return std::copysign(UnsignedUnwrappedAngle, SignedWrappedAngle);


	}

	struct SPolarPoint
	{
		SPolarPoint() = default;

		SPolarPoint(double radius, double angle) :
			Radius{ radius },
			Angle{ angle }
		{
		}

		double Radius;
		double Angle;

		//Transforms the point into the polar system of this point's parent
		//(this point and the input point are expressed in different polar systems)
		SPolarPoint InverseTransform(const SPolarPoint &Point) const
		{
			SPolarPoint Out{};

			//At 180 degree the 'triangle' becomes colinear with the line from the
			//reference point to its coordinate system's origin
			//todo: add geometric examples
			if (Point.Angle < 180 + ReqPrecision && Point.Angle > 180 - ReqPrecision)
			{
				Out.Angle = Angle;
				Out.Radius = Point.Radius + Radius;
				return Out;

			}

			auto WrappedAngle{ std::fmod(Point.Angle, 180) };
			Out.Radius = sqrt(Point.Radius * Point.Radius + Radius * Radius - 2 * Point.Radius * Radius * cos(WrappedAngle * ToRadCoeff));
			
			//null term should not be possible if none of the Radii is null and Angle != 180
			auto Divisor{ 2 * Out.Radius * Radius };

			Out.Angle = acos
						(							
							(Radius * Radius + Out.Radius * Out.Radius - Point.Radius * Point.Radius) 
							/ Divisor							
						) * ToDegCoeff;

			return Out;


		}

		//Transforms the point into the polar system with this polar point at its origin
		//(this and the input point are expressed in the same polar system)
		SPolarPoint Transform(const SPolarPoint &Point) const
		{
			SPolarPoint Out{};

			auto UnsignedBiasedAngle{ std::abs(Point.Angle - Angle) };

			if (UnsignedBiasedAngle < 180 + ReqPrecision && UnsignedBiasedAngle > 180 - ReqPrecision)
			{
				Out.Angle = 0;
				Out.Radius = Radius + Point.Radius;
				return Out;

			}

			auto CleanAngle{ UnsignedBiasedAngle > 180 ? 360 - UnsignedBiasedAngle : UnsignedBiasedAngle };

			Out.Radius = sqrt(Point.Radius * Point.Radius + Radius * Radius - 2 * Point.Radius * Radius * cos(CleanAngle * ToRadCoeff));

			//null term should not be possible if none of the Radii is null and Angle != 180
			auto Divisor{ 2 * Out.Radius * Radius };

			Out.Angle = acos
			(
				(Radius * Radius + Out.Radius * Out.Radius - Point.Radius * Point.Radius)
				/ Divisor
			) * ToDegCoeff;

			return Out;



		}

		/*

		CPolarCollider Transform(const CPolarCollider &Collider) const
		{



		}
		*/



	};

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

	class CPolarCollider
	{
	public:
		CPolarCollider(const SPolarPoint &LowrangeCenter, double HalfWidth, double Depth) :
			m_LowrangeCenter{ LowrangeCenter }
		{
			m_HalfWidth = HalfWidth < 0 ? 0 : (HalfWidth > 180 ? 180 : HalfWidth);
			m_Depth = std::abs(Depth);

			m_ChunkPoints.LeftMin.Radius = m_ChunkPoints.RightMin.Radius = m_LowrangeCenter.Radius;
			m_ChunkPoints.LeftMax.Radius = m_ChunkPoints.RightMax.Radius = m_LowrangeCenter.Radius + Depth;

			m_ChunkPoints.LeftMin.Angle = m_ChunkPoints.LeftMax.Angle = std::fmod(m_LowrangeCenter.Radius + HalfWidth, 360);

			auto UnsafeAngle{ std::fmod(m_LowrangeCenter.Radius - HalfWidth, 360) };
			if (UnsafeAngle < 0)
			{
				UnsafeAngle = 360 - std::abs(UnsafeAngle);

			}
			m_ChunkPoints.LeftMin.Angle = m_ChunkPoints.LeftMax.Angle = UnsafeAngle;
						

		}

		CPolarCollider(const SPolarPoint (& aBoundingPoints)[4], const SPolarPoint &TrueCenter)
		{
			const SPolarPoint *aAngleSorted[4]{ &aBoundingPoints[0], &aBoundingPoints[1], &aBoundingPoints[2], &aBoundingPoints[3]};
			const SPolarPoint *aRadiusSorted[4]{ &aBoundingPoints[0], &aBoundingPoints[1], &aBoundingPoints[2], &aBoundingPoints[3] };

			/*
			std::sort(std::begin(aRadiusSorted), std::end(aRadiusSorted), [](const SPolarPoint &Lhs, const SPolarPoint &Rhs) { return Lhs.Radius < Rhs.Radius; });
			std::sort(std::begin(aAngleSorted), std::end(aAngleSorted), [](const SPolarPoint &Lhs, const SPolarPoint &Rhs) 
			{
				return Lhs.Angle < Rhs.Angle; 
			});
			*/
			auto SmallestRadius{ aRadiusSorted[3]->Radius };
			auto LargestRadius{ aRadiusSorted[0]->Radius };

			//auto 

			//m_ChunkPoints.LeftMin = SPolarPoint{}


		}
				
		bool DoCollidersIntersect(const CPolarCollider &Other)
		{
			auto PolarDist{ GetShortestAngleBetween(this->GetPolarCenter(), Other.GetPolarCenter()) };
			
			if
			(
				(
					this->GetRadiusMax() < Other.GetRadiusMin()
					|| this->GetRadiusMin() > Other.GetRadiusMax()
				)
				||	PolarDist > Other.GetHalfPolarWidth() + this->GetHalfPolarWidth()
			)
			{
				return false;
			}
			else
			{
				return true;
			}		


		}

		double GetRadiusMin() const
		{
			return m_LowrangeCenter.Radius;


		}

		double GetRadiusMax() const
		{
			return m_LowrangeCenter.Radius + m_Depth;


		}

		double GetHalfPolarWidth() const
		{
			return m_HalfWidth;


		}

		double GetPolarCenter() const
		{
			return m_LowrangeCenter.Angle;


		}

		SPolarPoint GetLeftMin() const
		{
			return m_ChunkPoints.LeftMin;


		}

		SPolarPoint GetLeftMax() const
		{
			return m_ChunkPoints.LeftMax;


		}
		
		SPolarPoint GetRightMin() const
		{
			return m_ChunkPoints.RightMin;


		}

		SPolarPoint GetRightMax() const
		{
			return m_ChunkPoints.RightMax;


		}


	protected:		
		SPolarPoint m_LowrangeCenter;
		double	m_HalfWidth,
				m_Depth;

		union
		{
			SPolarPoint LeftMin,
						RightMin,
						LeftMax,
						RightMax;
			SPolarPoint Mem[4];
		}m_ChunkPoints;
	

	};


}

