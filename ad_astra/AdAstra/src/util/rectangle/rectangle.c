#include "rectangle.h"


bool areIntersecting(Rectangle r1, Rectangle r2) {
	return (
		r1.position.x < r2.position.x + r2.size.x &&
		r1.position.x + r1.size.x > r2.position.x &&
		r1.position.y < r2.position.y + r2.size.y &&
		r1.position.y + r1.size.y > r2.position.y
	);
}

bool isInside(Rectangle inner, Rectangle outer) {
	return (
		outer.position.x <= inner.position.x && 
		inner.position.x + inner.size.x <= outer.position.x + outer.size.x &&
		outer.position.y <= inner.position.y &&
		inner.position.y + inner.size.y <= outer.position.y + outer.size.y
	);
}

Vec2 getCenter(Rectangle r) {
	return (Vec2) {
		r.position.x + r.size.x / 2,
		r.position.y + r.size.y / 2
	};
}

Rectangle translateRectangle(Rectangle r, Vec2 translate) {
	return (Rectangle) {
		add(r.position, translate),
		r.size
	};
}
