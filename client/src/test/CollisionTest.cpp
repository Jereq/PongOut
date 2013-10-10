#include <boost/test/unit_test.hpp>

#include "PhysicsComponent.h"
#include <glm/glm.hpp>
BOOST_AUTO_TEST_SUITE(CollisionTestSuite)

BOOST_AUTO_TEST_CASE(TestCircleCircleIntersection)
{
	glm::vec2	bc1(100, 100);
	float		br1 = 20.f;
	glm::vec2	bc2(200, 200);
	float		br2 = 30.f;
	glm::vec2	bc3(120, 100);
	float		br3 = 0.f;

	BOOST_CHECK(PhysicsComponent::circleCirleIntersect(bc1, br1, bc2, br2) == false);
	BOOST_CHECK(PhysicsComponent::circleCirleIntersect(bc1, br1, bc3, br3) == true);
	BOOST_CHECK(PhysicsComponent::circleCirleIntersect(bc2, br2, bc3, br3) == false);
	BOOST_CHECK(PhysicsComponent::circleCirleIntersect(bc1, br1, bc1, br2) == true);
}

BOOST_AUTO_TEST_CASE(TestCirleLineIntersection)
{
	BOOST_CHECK(PhysicsComponent::circleLineIntersect(glm::vec2(10.f, 30.f), 10.f, glm::vec2(0.f, 0.f), glm::vec2(20.f, 60.f)) == true);
	BOOST_CHECK(PhysicsComponent::circleLineIntersect(glm::vec2(10.f, 30.f), 10.f, glm::vec2(0.f, 0.f), glm::vec2(9.f, 29.f)) == false);
	BOOST_CHECK(PhysicsComponent::circleLineIntersect(glm::vec2(10.f, 30.f), 10.f, glm::vec2(0.f, 0.f), glm::vec2(0.f, 60.f)) == true);
	BOOST_CHECK(PhysicsComponent::circleLineIntersect(glm::vec2(10.f, 30.f), 10.f, glm::vec2(0.f, 0.f), glm::vec2(-1.f, 60.f)) == false);
}

BOOST_AUTO_TEST_SUITE_END()
