#pragma once
#include <memory>
#include <forward_list>
#include <functional>

namespace RadialMath
{
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

		/*
		template<class functor_type, class decay_type = std::decay_t<functor_type> >
		static TDelegate Create(functor_type &&Functor)
		{
			return TDelegate( Functor, &FunctorCaller<decay_type> );


		}
		*/
					   		 	  
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

		/*
		template<class functor_type>
		static return_type FunctorCaller(void *pFunctor, arg_types ...Args)
		{
			return static_cast<functor_type *>(pFunctor)(Args ...);


		}
		*/

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

	struct SRadialTransform
	{		
		SRadialPoint TransformRadialPoint(const SRadialPoint &ToTransform)
		{
			SRadialPoint Transformed{}, ToTransformTransformed{};

			
			auto *pNextParent{ GetParent() };
			auto *pNextParentToTransform{ ToTransform.GetParent() };
			while (pNextParent != pNextParentToTransform)
			{



				pNextParent = pNextParent.GetParent();
				pNextParentToTransform = pNextParentToTransform.GetParent();
			}
			

			//Transformed.Radius = 

		}
		

		void AttachChild(SRadialTransform &ToAttach)
		{
			m_ChildrenList.push_front(ToAttach);


		}

		void DetachChild(SRadialTransform &ToDetach)
		{
			m_ChildrenList.remove(ToDetach);


		}

		SRadialTransform *GetParent() const noexcept
		{
			return pParent;


		}

		SRadialPoint GetOrigin() { SRadialPoint Out{}; Out.Angle = Angle; Out.Radius = Radius; return Out; }

		double Radius;
		double Angle;


	protected:
		SRadialTransform *pParent;
		std::forward_list<SRadialTransform *> m_ChildrenList;
	

	};

	struct SRadialPoint
	{
		double Radius;
		double Angle;

	};

}

