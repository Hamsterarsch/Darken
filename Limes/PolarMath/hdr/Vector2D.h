#pragma once


namespace PolarMath
{
	struct SVector2D
	{
	public:
		SVector2D() = default;

		constexpr SVector2D(double x, double y) :
			Mem{ x, y }
		{}
			   
		double GetLength() const;
		double Dot(const SVector2D &Other) const;

		void Normalize();
		SVector2D GetNormalized() const;
		struct SPolarPoint ToPolar() const;
		double GetAngleTo(const SVector2D &Other) const;

		SVector2D &operator+=(const SVector2D &Rhs);
		SVector2D &operator-=(const SVector2D &Rhs);
		SVector2D &operator*=(double Scalar);
		
		bool operator==(const SVector2D &Rhs) const;

		static const SVector2D AxisX;
		static const SVector2D AxisY;

		union
		{
			struct 
			{				
				double	X,
						Y;
			};
			double Mem[2];
		};
		

	};

	SVector2D operator+(SVector2D Lhs, const SVector2D &Rhs);
	SVector2D operator-(SVector2D Lhs, const SVector2D &Rhs);
	SVector2D operator*(SVector2D Lhs, double Scalar);

	SPolarPoint ToPolar(SVector2D Vector);
	

}
