
#include <GameLibrary/Physics/CollisionRects/CollisionRect.hpp>
#include <GameLibrary/Physics/CollisionRects/BoxCollisionRect.hpp>

namespace fgl
{
	CollisionRect::CollisionRect(const String& tag)
		: tag(tag)
	{
		//
	}

	CollisionRect::~CollisionRect()
	{
		//
	}

	const String& CollisionRect::getTag() const
	{
		return tag;
	}

	Vector2d CollisionRect::getCenter() const
	{
		return getRect().getCenter();
	}
	
	Vector2d CollisionRect::getTotalVelocity() const
	{
		/*auto rect = getRect();
		auto prevRect = getPreviousRect();
		double velocityLeft = rect.x - prevRect.x;
		double velocityTop = rect.y - prevRect.y;
		double velocityRight = (rect.x+rect.width) - (prevRect.x+prevRect.width);
		double velocityBottom = (rect.y+rect.height) - (prevRect.y+prevRect.height);
		return Vector2d(velocityTop+velocityBottom, velocityLeft+velocityRight);*/
		return getRect().getCenter() - getPreviousRect().getCenter();
	}

	Vector2d CollisionRect::getCollisionOffset(CollisionRect* collisionRect1, CollisionRect* collisionRect2)
	{
		RectangleD rect1 = collisionRect1->getRect();
		RectangleD rect2 = collisionRect2->getRect();
		if(rect1.intersects(rect2))
		{
			bool filled1 = collisionRect1->isFilled();
			bool filled2 = collisionRect2->isFilled();
			if(filled1 && filled2)
			{
				return getFilledCollisionOffset(collisionRect1, collisionRect2);
			}
			else if(filled1 || filled2)
			{
				CollisionRect* filledRect = nullptr;
				CollisionRect* pixelRect = nullptr;
				if(filled1)
				{
					filledRect = collisionRect1;
					pixelRect = collisionRect2;
				}
				else //if(filled2)
				{
					filledRect = collisionRect2;
					pixelRect = collisionRect1;
				}
				Vector2d offset = getPixelOnFilledCollisionOffset(pixelRect, filledRect);
				if(filled1)
				{
					offset.x = -offset.x;
					offset.y = -offset.y;
				}
				return offset;
			}
			else
			{
				return getPixelCollisionOffset(collisionRect1, collisionRect2);
			}
		}
		return Vector2d(0, 0);
	}

	typedef enum
	{
		DIR_NONE,
		DIR_UP,
		DIR_LEFT,
		DIR_DOWN,
		DIR_RIGHT
	} CollisionRectDirection;

	CollisionRectDirection CollisionRect_findRectCollisionDirection(const RectangleD& rect1, const RectangleD& prevRect1, const RectangleD& rect2, const RectangleD& prevRect2)
	{
		if(!rect1.intersects(rect2))
		{
			//no collision happening
			return DIR_NONE;
		}
		
		double velocityLeft1 = rect1.x - prevRect1.x;
		double velocityTop1 = rect1.y - prevRect1.y;
		double velocityRight1 = (rect1.x+rect1.width) - (prevRect1.x+prevRect1.width);
		double velocityBottom1 = (rect1.y+rect1.height) - (prevRect1.y+prevRect1.height);
		
		double velocityLeft2 = rect2.x - prevRect2.x;
		double velocityTop2 = rect2.y - prevRect2.y;
		double velocityRight2 = (rect2.x+rect2.width) - (prevRect2.x+prevRect2.width);
		double velocityBottom2 = (rect2.y+rect2.height) - (prevRect2.y+prevRect2.height);

		unsigned long multiplyNum = 1;
		do
		{
			double velocityLeft = velocityLeft1 - velocityRight2;
			double velocityRight = velocityRight1 - velocityLeft2;
			double velocityTop = velocityTop1 - velocityBottom2;
			double velocityBottom = velocityBottom1 - velocityTop2;
			velocityLeft *= (double)multiplyNum;
			velocityRight *= (double)multiplyNum;
			velocityTop *= (double)multiplyNum;
			velocityBottom *= (double)multiplyNum;
			if(multiplyNum > 20 || (velocityLeft==0 && velocityRight==0 && velocityTop==0 && velocityBottom==0))
			{
				RectangleD intersect1 = rect1.getIntersect(rect2);
				double intersect1_right = intersect1.x + intersect1.width;
				double intersect1_bottom = intersect1.y + intersect1.height;
				double rect1_right = rect1.x + rect1.width;
				double rect1_bottom = rect1.y + rect1.height;

				double difRight = rect1_right- intersect1_right;
				double difLeft = intersect1.x - rect1.x;
				double difBottom = rect1_bottom - intersect1_bottom;
				double difTop = intersect1.y - rect1.y;

				double difX = 0;
				double difY = 0;
				CollisionRectDirection dirX = DIR_NONE;
				CollisionRectDirection dirY = DIR_NONE;
				if(difLeft < difRight)
				{
					difX = difLeft;
					dirX = DIR_LEFT;
				}
				else if(difRight < difLeft)
				{
					difX = difRight;
					dirX = DIR_RIGHT;
				}
				
				if(difTop < difBottom)
				{
					difY = difTop;
					dirY = DIR_UP;
				}
				else if(difBottom < difTop)
				{
					difY = difBottom;
					dirY = DIR_DOWN;
				}

				if(dirX==DIR_NONE)
				{
					if(dirY==DIR_NONE)
					{
						//TODO have a fallback rather than just returning up
						return DIR_UP;
					}
					else
					{
						return dirY;
					}
				}
				else if(dirY==DIR_NONE)
				{
					return dirX;
				}
				else
				{
					if(difX < difY)
					{
						return dirX;
					}
					else if(difY < difX)
					{
						return dirY;
					}
					else
					{
						if(intersect1.width > intersect1.height)
						{
							return dirX;
						}
						else if(intersect1.height > intersect1.width)
						{
							return dirY;
						}
						return dirY;
					}
				}
			}

			double rect1_right = rect1.x + rect1.width;
			double rect1_bottom = rect1.y + rect1.height;

			RectangleD pastRect2 = prevRect2;
			pastRect2.x += velocityRight1;
			pastRect2.width -= velocityRight1;
			pastRect2.width += velocityLeft1;
			pastRect2.y += velocityBottom1;
			pastRect2.height -= velocityBottom1;
			pastRect2.height += velocityTop1;
			double pastRect2_right = pastRect2.x+pastRect2.width;
			double pastRect2_bottom = pastRect2.y+pastRect2.height;

			if(pastRect2_bottom <= rect1.y)
				//above frame1
			{
				if(pastRect2_right < rect1.x)
					//top left
				{
					LineD lineBL(pastRect2.x, pastRect2_bottom, pastRect2.x-velocityLeft, pastRect2_bottom-velocityBottom);
					LineD lineBR(pastRect2_right, pastRect2_bottom, pastRect2_right-velocityRight, pastRect2_bottom-velocityBottom);
					LineD lineTR(pastRect2_right, pastRect2.y, pastRect2_right-velocityRight, pastRect2.y-velocityTop);

					Vector2d box_TR(rect1_right, rect1.y);
					Vector2d box_TL(rect1.x, rect1.y);
					Vector2d box_BL(rect1.x, rect1_bottom);

					if(lineBR.segmentsIntersect(LineD(box_TL, box_TR)))
					{
						//push up
						return DIR_UP;
					}
					if(lineBR.segmentsIntersect(LineD(box_TL, box_BL)))
					{
						//push left
						return DIR_LEFT;
					}

					PolygonD polyRight;
					polyRight.addPoint(lineTR.point1);
					polyRight.addPoint(lineTR.point2);
					polyRight.addPoint(lineBR.point2);
					polyRight.addPoint(lineBR.point1);
					if(polyRight.contains(box_BL) || polyRight.contains(box_TL))
					{
						//push left
						return DIR_LEFT;
					}
					//push up
					return DIR_UP;
				}
				else if(pastRect2.x > rect1_right)
					//top right
				{
					LineD lineBR(pastRect2_right, pastRect2_bottom, pastRect2_right-velocityRight, pastRect2_bottom-velocityBottom);
					LineD lineBL(pastRect2.x, pastRect2_bottom, pastRect2.x-velocityLeft, pastRect2_bottom-velocityBottom);
					LineD lineTL(pastRect2.x, pastRect2.y, pastRect2.x-velocityLeft, pastRect2.y-velocityTop);

					Vector2d box_TL(rect1.x, rect1.y);
					Vector2d box_TR(rect1_right, rect1.y);
					Vector2d box_BR(rect1_right, rect1_bottom);

					if(lineBL.segmentsIntersect(LineD(box_TL, box_TR)))
					{
						//push up
						return DIR_UP;
					}
					if(lineBL.segmentsIntersect(LineD(box_TR, box_BR)))
					{
						//push right
						return DIR_RIGHT;
					}

					PolygonD polyLeft;
					polyLeft.addPoint(lineTL.point1);
					polyLeft.addPoint(lineTL.point2);
					polyLeft.addPoint(lineBL.point2);
					polyLeft.addPoint(lineBL.point1);
					if(polyLeft.contains(box_BR) || polyLeft.contains(box_TR))
					{
						//push right
						return DIR_RIGHT;
					}
					//push up
					return DIR_UP;
				}
				else
					//top middle
				{
					//push up
					return DIR_UP;
				}
			}
			else if(pastRect2.y >= rect1_bottom)
				//below frame1
			{
				if(pastRect2_right <= rect1.x)
					//bottom left
				{
					LineD lineBR(pastRect2_right, pastRect2_bottom, pastRect2_right-velocityRight, pastRect2_bottom-velocityBottom);
					LineD lineTR(pastRect2_right, pastRect2.y, pastRect2_right-velocityRight, pastRect2.y-velocityTop);
					LineD lineTL(pastRect2.x, pastRect2.y, pastRect2.x-velocityLeft, pastRect2.y-velocityTop);

					Vector2d box_TL(rect1.x, rect1.y);
					Vector2d box_BL(rect1.x, rect1_bottom);
					Vector2d box_BR(rect1_right, rect1_bottom);

					if(lineTR.segmentsIntersect(LineD(box_BL, box_BR)))
					{
						//push down
						return DIR_DOWN;
					}
					if(lineTR.segmentsIntersect(LineD(box_TL, box_BL)))
					{
						//push left
						return DIR_LEFT;
					}

					PolygonD polyRight;
					polyRight.addPoint(lineTR.point1);
					polyRight.addPoint(lineTR.point2);
					polyRight.addPoint(lineBR.point2);
					polyRight.addPoint(lineBR.point1);
					if(polyRight.contains(box_TL) || polyRight.contains(box_BL))
					{
						//push left
						return DIR_LEFT;
					}
					//push down
					return DIR_DOWN;
				}
				else if(pastRect2.x >= rect1_right)
					//bottom right
				{
					LineD lineBL(pastRect2.x, pastRect2_bottom, pastRect2.x-velocityLeft, pastRect2_bottom-velocityBottom);
					LineD lineTL(pastRect2.x, pastRect2.y, pastRect2.x-velocityLeft, pastRect2.y-velocityTop);
					LineD lineTR(pastRect2_right, pastRect2.y, pastRect2_right-velocityRight, pastRect2.y-velocityTop);

					Vector2d box_TR(rect1_right, rect1.y);
					Vector2d box_BR(rect1_right, rect1_bottom);
					Vector2d box_BL(rect1.x, rect1_bottom);

					if(lineTL.segmentsIntersect(LineD(box_BL, box_BR)))
					{
						//push down
						return DIR_DOWN;
					}
					if(lineTL.segmentsIntersect(LineD(box_TR, box_BR)))
					{
						//push right
						return DIR_RIGHT;
					}

					PolygonD polyLeft;
					polyLeft.addPoint(lineTL.point1);
					polyLeft.addPoint(lineTL.point2);
					polyLeft.addPoint(lineBL.point2);
					polyLeft.addPoint(lineBL.point1);
					if(polyLeft.contains(box_TR) || polyLeft.contains(box_BR))
					{
						//push right
						return DIR_RIGHT;
					}
					//push down
					return DIR_DOWN;
				}
				else
					//bottom middle
				{
					//push down
					return DIR_DOWN;
				}
			}
			else
				//within top and bottom of frame 1
			{
				if(pastRect2_right <= rect1.x)
					//middle left
				{
					//push left
					return DIR_LEFT;
				}
				else if(pastRect2.x >= rect1_right)
					//middle right
				{
					//push right
					return DIR_RIGHT;
				}
				else
				{
					//The rest is inside of the box, try a bigger velocity. Loop again.
				}
			}
			multiplyNum++;
		}
		while(true);
		return DIR_NONE;
	}
	
	Vector2d CollisionRect_getRectCollisionOffset(const RectangleD& rect1, const RectangleD& prevRect1, const RectangleD& rect2, const RectangleD& prevRect2)
	{
		CollisionRectDirection dir = CollisionRect_findRectCollisionDirection(rect1, prevRect1, rect2, prevRect2);
		switch(dir)
		{
			case DIR_NONE:
				return Vector2d(0, 0);
				
			case DIR_UP:
				return Vector2d(0, rect1.y-(rect2.y+rect2.height));
				
			case DIR_DOWN:
				return Vector2d(0, (rect1.y+rect1.height)-rect2.y);
				
			case DIR_LEFT:
				return Vector2d(rect1.x-(rect2.x+rect2.width), 0);
				
			case DIR_RIGHT:
				return Vector2d((rect1.x+rect1.width)-rect2.x, 0);
		}
		return Vector2d(0, 0);
	}
	
	
	
	
	
	Vector2d CollisionRect::getFilledCollisionOffset(CollisionRect* collisionRect1, CollisionRect* collisionRect2)
	{
		return CollisionRect_getRectCollisionOffset(collisionRect1->getRect(), collisionRect1->getPreviousRect(), collisionRect2->getRect(), collisionRect2->getPreviousRect());
	}
	
	CollisionRectDirection CollisionRect_getPointDirection(const Vector2d& point1, const Vector2d& point2)
	{
		double dir = Math::normalizeDegrees(Math::radtodeg(Math::atan2(-(point2.y-point1.y), point2.x-point1.x)));
		if(dir <= 45 || dir >= 315)
		{
			return DIR_RIGHT;
		}
		else if(dir >= 45 && dir <= 135)
		{
			return DIR_UP;
		}
		else if(dir >= 135 && dir<=225)
		{
			return DIR_LEFT;
		}
		else if(dir >= 225 && dir <= 315)
		{
			return DIR_DOWN;
		}
		return DIR_NONE;
	}

	CollisionRectDirection CollisionRect_getRectDirection(const RectangleD& r1, const RectangleD& r2)
	{
		float rx2 = (float)((float)r2.x + (float)r2.width/2) - (float)((float)r1.x + (float)r1.width/2);
		float ry2 = (float)((float)r2.y + (float)r2.height/2) - (float)((float)r1.y + (float)r1.height/2);

		if(ry2<0) //r2 is above r1
		{
			if(rx2 == 0)
			{
				return DIR_UP;
			}
			else
			{
				float ratLeft = (float)(-((float)r1.height/2))/(float)(-((float)r1.width/2));
				float ratRight = (float)(-((float)r1.height/2))/(float)(((float)r1.width/2));

				float difRat =  ry2/rx2;

				if(difRat==ratRight)
				{
					return DIR_UP;
				}
				else if(difRat==ratLeft)
				{
					return DIR_UP;
				}
				else if((difRat>ratRight)&&(difRat<0))
				{
					return DIR_RIGHT;
				}
				else if((difRat<ratLeft)&&(difRat>0))
				{
					return DIR_LEFT;
				}
				else
				{
					return DIR_UP;
				}
			}
		}
		else if(ry2>0)//r2 is below r1
		{
			if(rx2 == 0)
			{
				return DIR_DOWN;
			}
			else
			{
				float ratLeft = (float)(((float)r1.height/2))/(float)(-((float)r1.width/2));
				float ratRight = (float)(((float)r1.height/2))/(float)(((float)r1.width/2));

				float difRat =  ry2/rx2;

				if(difRat==ratRight)
				{
					return DIR_DOWN;// RIGHT;
				}
				else if(difRat==ratLeft)
				{
					return DIR_DOWN;// LEFT;
				}
				else if((difRat<ratRight)&&(difRat>0))
				{
					return DIR_RIGHT;
				}
				else if((difRat>ratLeft)&&(difRat<0))
				{
					return DIR_LEFT;
				}
				else
				{
					return DIR_DOWN;
				}
			}
		}
		else
		{
			if(rx2<0)
			{
				return DIR_LEFT;
			}
			else if(rx2>0)
			{
				return DIR_RIGHT;
			}
			else
			{
				return DIR_UP;
			}
		}
	}

	Vector2d CollisionRect::getPixelOnFilledCollisionOffset(CollisionRect* pixelRect, CollisionRect* filledRect)
	{
		auto rect1 = pixelRect->getRect();
		auto rect2 = filledRect->getRect();
		auto overlap = rect1.getIntersect(rect2);
		if(overlap.width==0 || overlap.height==0)
		{
			return Vector2d(0,0);
		}
		
		Vector2d increment1 = pixelRect->getPreferredIncrement();
		Vector2d increment2 = filledRect->getPreferredIncrement();
		Vector2d increment = Vector2d(Math::min(increment1.x, increment2.x), Math::min(increment1.x, increment2.x));
		
		RectD pixelArea;
		bool colliding = false;
		
		auto pixelIter = pixelRect->createPixelIterator(overlap, increment);
		
		while(pixelIter.nextPixelIndex())
		{
			if(pixelRect->check(pixelIter))
			{
				Vector2d point = pixelIter.getCurrentPoint();
				double pointRight = point.x + increment1.x;
				double pointBottom = point.y + increment1.y;
				if(!colliding)
				{
					pixelArea.left = point.x;
					pixelArea.right = pointRight;
					pixelArea.top = point.y;
					pixelArea.bottom = pointBottom;
				}
				else
				{
					if(point.x < pixelArea.left)
					{
						pixelArea.left = point.x;
					}
					else if(pointRight > pixelArea.right)
					{
						pixelArea.right = pointRight;
					}
					if(point.y < pixelArea.top)
					{
						pixelArea.top = point.y;
					}
					else if(pointBottom > pixelArea.bottom)
					{
						pixelArea.bottom = pointBottom;
					}
				}
				colliding = true;
			}
		}
		
		if(colliding)
		{
			auto pixelOverlap = pixelArea.toRectangle();
			CollisionRectDirection dir = CollisionRect_getRectDirection(rect2, pixelOverlap);
			
			switch(dir)
			{
				case DIR_NONE:
					return Vector2d(0, 0);
					
				case DIR_DOWN:
					return Vector2d(0, pixelOverlap.y-(rect2.y+rect2.height));
					
				case DIR_UP:
					return Vector2d(0, (pixelOverlap.y+pixelOverlap.height)-rect2.y);
					
				case DIR_RIGHT:
					return Vector2d(pixelOverlap.x-(rect2.x+rect2.width), 0);
					
				case DIR_LEFT:
					return Vector2d((pixelOverlap.x+pixelOverlap.width)-rect2.x, 0);
			}
		}
		return Vector2d(0,0);
	}

	Vector2d CollisionRect::getPixelCollisionOffset(CollisionRect* collisionRect1, CollisionRect* collisionRect2)
	{
		//TODO implement pixel on pixel collisions
		return Vector2d(0, 0);
	}
	
	
	
	
	bool CollisionRect::checkCollision(CollisionRect* collisionRect1, CollisionRect* collisionRect2)
	{
		RectangleD rect1 = collisionRect1->getRect();
		RectangleD rect2 = collisionRect2->getRect();
		if(rect1.intersects(rect2))
		{
			bool filled1 = collisionRect1->isFilled();
			bool filled2 = collisionRect2->isFilled();
			if(filled1 && filled2)
			{
				return checkFilledCollision(collisionRect1, collisionRect2);
			}
			else if(filled1 || filled2)
			{
				CollisionRect* filledRect = nullptr;
				CollisionRect* pixelRect = nullptr;
				if(filled1)
				{
					filledRect = collisionRect1;
					pixelRect = collisionRect2;
				}
				else //if(filled2)
				{
					filledRect = collisionRect2;
					pixelRect = collisionRect1;
				}
				return checkPixelOnFilledCollision(pixelRect, filledRect);
			}
			else
			{
				return checkPixelCollision(collisionRect1, collisionRect2);
			}
		}
		return false;
	}

	bool CollisionRect::checkCollision(CollisionRect* collisionRect, const RectangleD& filledRect)
	{
		BoxCollisionRect filledBoxRect("", filledRect, filledRect);
		return checkCollision(collisionRect, &filledBoxRect);
	}
	
	bool CollisionRect::checkFilledCollision(CollisionRect* collisionRect1, CollisionRect* collisionRect2)
	{
		if(collisionRect1->getRect().intersects(collisionRect2->getRect()))
		{
			return true;
		}
		return false;
	}
	
	bool CollisionRect::checkPixelOnFilledCollision(CollisionRect* pixelRect, CollisionRect* filledRect)
	{
		auto rect1 = pixelRect->getRect();
		auto rect2 = filledRect->getRect();
		auto intersect = rect1.getIntersect(rect2);
		if(intersect.width==0 || intersect.height==0)
		{
			return false;
		}
		Vector2d increment1 = pixelRect->getPreferredIncrement();
		Vector2d increment2 = filledRect->getPreferredIncrement();
		Vector2d increment = Vector2d(Math::min(increment1.x, increment2.x), Math::min(increment1.x, increment2.x));
		PixelIterator pixelIter1 = pixelRect->createPixelIterator(intersect, increment);
		while(pixelIter1.nextPixelIndex())
		{
			if(pixelRect->check(pixelIter1))
			{
				return true;
			}
		}
		return false;
	}
	
	bool CollisionRect::checkPixelCollision(fl::CollisionRect* collisionRect1, fl::CollisionRect* collisionRect2)
	{
		auto rect1 = collisionRect1->getRect();
		auto rect2 = collisionRect2->getRect();
		auto intersect = rect1.getIntersect(rect2);
		if(intersect.width==0 || intersect.height==0)
		{
			return false;
		}
		Vector2d increment1 = collisionRect1->getPreferredIncrement();
		Vector2d increment2 = collisionRect2->getPreferredIncrement();
		Vector2d increment = Vector2d(Math::min(increment1.x, increment2.x), Math::min(increment1.x, increment2.x));
		PixelIterator pixelIter1 = collisionRect1->createPixelIterator(intersect, increment);
		PixelIterator pixelIter2 = collisionRect2->createPixelIterator(intersect, increment);
		while(pixelIter1.nextPixelIndex() && pixelIter2.nextPixelIndex())
		{
			if(collisionRect1->check(pixelIter1) && collisionRect2->check(pixelIter2))
			{
				return true;
			}
		}
		return false;
	}
}
