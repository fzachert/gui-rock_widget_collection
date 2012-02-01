#include "BoundarySlider.h"
#include "SlideBarItem.h"

#include<iostream>

BoundarySlider::BoundarySlider() : Slider() {

}

BoundarySlider::BoundarySlider(QGraphicsItem* parent, QPointF initPos) : Slider(parent, initPos, 0) {
    setPixmap(QPixmap(":/timeline/triangle_slider"));
    setPointingOffset(width()/2.0);
    
    /* Set item coordinate origin to the center */
    setOffset( -0.5 * QPointF( width(), height() ) );
}

Direction BoundarySlider::leftRight(BoundarySlider *otherMarker) {
    if(this->getInitPos().x() > otherMarker->getInitPos().x()) {
        return RIGHT;
    }
    else {
        return LEFT;
    }
}

void BoundarySlider::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    /* Sliding bounds in parent's coords */
    qreal leftBoundary = parentItem()->boundingRect().left();
    qreal rightBoundary = parentItem()->boundingRect().right();

    QPointF newPos = boundarySnapPos(event->pos(), getClickPosOffsetX(event), leftBoundary, rightBoundary);

    // Check for marker overlap with new position
    setPos(newPos);
    if( BoundarySlider *overlappingMarker = ((SlideBarItem*) parentItem())->markerOverlap(this) ) {
        /* Collision detected! Do not move further in this direction! */

        std::cout << "Overlapping time markers!" << std::endl;

        qreal overlapLeftBoundary;
        qreal overlapRightBoundary;
        switch(leftRight(overlappingMarker)) {
        case LEFT:
            // the overlapping marker is right hand side
            overlapLeftBoundary = leftBoundary;
            overlapRightBoundary = overlappingMarker->pos().x();
            break;
        case RIGHT:
            // the overlapping marker is left hand side
            overlapLeftBoundary = overlappingMarker->pos().x() + overlappingMarker->width();
            overlapRightBoundary = rightBoundary;
            break;
        default :
            std::cerr << "Unknown direction!" << std::endl;
            exit(EXIT_FAILURE);
        }
        
        /* Update position */
        setPos(boundarySnapPos(event->pos(), getClickPosOffsetX(event), overlapLeftBoundary, overlapRightBoundary));

    } else {
        // No overlap, new position is safe to use.
    }
    unsigned index = ((SlideBarItem*)parentItem())->markerIndex(this);
    setLastIndex(index);
    
    std::cout << "Logical marker position: " << index << std::endl;
    //std::cout << "Computed real position for that index: " << ((SlideBarItem*)parentItem())->markerPositionForIndex(index) << std::endl;
    std::cout << "boundary slider pos.x: " << pos().x() << std::endl;
    parentItem()->update(); // updates index slider as well
//    update();
    
}
