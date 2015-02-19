
#ifndef CAFFEINE_MATH_TRANSFORM_INCLUDED
#define CAFFEINE_MATH_TRANSFORM_INCLUDED



#include <Caffeine/Math/Vector.hpp>
#include <Caffeine/Math/Quaternion.hpp>



namespace Caffeine {
namespace Math {



struct Transform
{
	Quaternion 	rotation;
	Vector3 	position = { { 0.f, 0.f, 0.f } };
	Vector3 	scale = { { 1.f, 1.f, 1.f } };
};



inline Transform TransformInitialize(const Vector3 &position, const Vector3 &scale)
{
	Transform newTrans;
	newTrans.position = position;
	newTrans.scale = scale;

	return newTrans;
}



inline Vector3 TransformGetRelativePosition(const Transform &parent, const Transform &child)
{
	const Vector3 offset = Vector3ComponentSubtract(child.position, parent.position);
	return offset;
}



inline Vector3 TransformGetRelativeScale(const Transform &parent, const Transform &child)
{
	const Vector3 relScale = Vector3ComponentMultiply(parent.scale, child.scale);
	return relScale;
}



inline Transform TransformScaleAndPosition(const Transform &trans, const Vector3 &scale)
{
	const Vector3 newScale 	  = Vector3ComponentMultiply(trans.scale, scale);
	const Vector3 newPosition = Vector3ComponentMultiply(trans.position, scale);

	Transform newTransform;
	newTransform.position 	= newPosition;
	newTransform.scale 		= newScale;

	return newTransform;
}



//inline Transform TransformScaleInRelation(const Transform &root, const Transform &toScale, const Vector3 &scale)
//{
//	
//}



} // namespace
} // namespace



#endif // include guard
