#include "lib_phys.h"

Intersection intersectLine(Vector2 start_a, Vector2 end_a, Vector2 start_b, Vector2 end_b) {
	Vector2 a = Vec2Sub(end_a, start_a);
	Vector2 b = Vec2Sub(end_b, start_b);
	float cross1 = Vec2CrossPoduct(a, b);
	float cross2 = Vec2CrossPoduct(b, a);
	if (fabs(cross1 - 0.0f) > 0.01) { //Float kann man nicht direkt auf 0 testen!!!
		float s = Vec2CrossPoduct(Vec2Sub(start_b, start_a), b) / cross1;
		float u = Vec2CrossPoduct(Vec2Sub(start_a, start_b), a) / cross2;
		if (s > 0.0001 && s < 1 && u > 0.0001 && u < 1) {
			return (Intersection){s, Vec2Add(start_a, Vec2Scale(a, s))};
		}
	}
	return (Intersection){0.0f, (Vector2){0.0f, 0.0f}};
}

void _drawBox(Shape *box, float thick, Color c) {
    for (int i = 0; i < 4; i++)
    {
        DrawLineEx(box->vertices[i], box->vertices[i + 1], thick, c);
    }
    DrawCircleV(box->location, 5, c);
}

void _rotateBox(Shape *box, float angle) {
    for (int i = 0; i < 5; i++)
    {
        box->vertices[i] = Vec2Rotate(box->vertices[i], box->location, angle);
    }
}

void _updateBox(Shape *box) {
    box->velocity = Vec2Add(box->velocity, box->accel);
    box->velocity =  Vec2Limit(box->velocity, 10.0f);
    box->accel = (Vector2){0.0f, 0.0f};

    box->angVelocity += box->angAccel;
    box->angVelocity = limitNum(box->angVelocity, 0.05f);
    box->angAccel = 0.0f;

    box->location = Vec2Add(box->location, box->velocity);
    box->vertices[0] = Vec2Add(box->vertices[0], box->velocity);
    box->vertices[1] = Vec2Add(box->vertices[1], box->velocity);
    box->vertices[2] = Vec2Add(box->vertices[2], box->velocity);
    box->vertices[3] = Vec2Add(box->vertices[3], box->velocity);
    box->vertices[4] = Vec2Add(box->vertices[4], box->velocity);

    _rotateBox(box, box->angVelocity);
}

void _resetPosBox(Shape *box, Vector2 v) {
    if (box->mass != INFINITY)
    {
        box->location = Vec2Add(box->location, v);
        box->vertices[0] = Vec2Add(box->vertices[0], v);
        box->vertices[1] = Vec2Add(box->vertices[1], v);
        box->vertices[2] = Vec2Add(box->vertices[2], v);
        box->vertices[3] = Vec2Add(box->vertices[3], v);
        box->vertices[4] = Vec2Add(box->vertices[4], v);
    }
}

void _drawBall(Shape *ball, float thick, Color c) {
    DrawRing(ball->location, ball->radius - 3, ball->radius, 0, 360, 1, c);
    // DrawCircleV(location.pos,radius, c);
    //DrawLineEx(ball->location, ball->orientation, thick, c);
}

void _rotateBall(Shape* ball, float angle) {
    ball->orientation = Vec2Rotate(ball->orientation, ball->location, angle);
}

void _updateBall(Shape* ball) {
    ball->velocity = Vec2Add(ball->velocity, ball->accel);
    ball->velocity = Vec2Limit(ball->velocity, 10.0f);
    ball->accel = (Vector2){0.0f, 0.0f};

    ball->angVelocity += ball->angAccel;
    ball->angVelocity = limitNum(ball->angVelocity, 0.05f);
    ball->angAccel = 0.0f;

    ball->location = Vec2Add(ball->location, ball->velocity);
    ball->orientation = Vec2Add(ball->orientation, ball->velocity);

    _rotateBall(ball, ball->angVelocity);
}

void _resetPosBall(Shape *ball, Vector2 v) {
    if (ball->mass != INFINITY) {
        ball->location = Vec2Add(ball->location, v);
        ball->orientation = Vec2Add(ball->orientation, v);
    }
}

Shadow createShadow(Shape* shape) {
    Shadow shadow;
    if (shape->typ == BALL) {
        shadow = (Shadow){
            .minX = shape->location.x - shape->radius, 
            .maxX = shape->location.x + shape->radius, 
            .minY = shape->location.y - shape->radius, 
            .maxY = shape->location.y + shape->radius
            };
    } else {
        shadow = (Shadow){
            .minX = INFINITY, 
            .maxX =-INFINITY, 
            .minY = INFINITY, 
            .maxY =-INFINITY
            };
        for (int i = 0; i < 4; i++) {
            if (shape->vertices[i].x < shadow.minX) {
                shadow.minX = shape->vertices[i].x;
            } 
            if (shape->vertices[i].y < shadow.minY) {
                shadow.minY = shape->vertices[i].y;
            } 
            if (shape->vertices[i].x > shadow.maxX) {
                shadow.maxX = shape->vertices[i].x;
            } 
            if (shape->vertices[i].y > shadow.maxY) {
                shadow.maxY = shape->vertices[i].y;
            } 
        }    
    }
    
    return shadow;
}

Shape Box(float x, float y, float w, float h) {
    Shape result = {
        .typ = BOX,
        .marked = false,
        .location = {x + w / 2, y + h / 2},
        .mass = (w + h) * 2,
        .inertia = w * h * w,
        .velocity = {0, 0},
        .angVelocity = 0,
        .accel = {0, 0},
        .angAccel = 0,
        .vertices[0] = {x, y},
        .vertices[1] = {x + w, y},
        .vertices[2] = {x + w, y + h},
        .vertices[3] = {x, y + h},
        .vertices[4] = {x, y},
        .funcDraw = &_drawBox,
        .funcUpdate = &_updateBox,
        .funcResetPos = &_resetPosBox
        };
    return result;
}

Shape Ball(float x, float y, float r) {
    Shape result = {
        .typ = BALL,
        .marked = false,
        .location = {x, y},
        .radius = r,
        .mass = r * 2,
        .inertia = r * r * r / 2,
        .velocity = {0, 0},
        .angVelocity = 0,
        .accel = {0, 0},
        .angAccel = 0,
        .orientation = {r + x, y},
        .funcDraw = &_drawBall,
        .funcUpdate = &_updateBall,
        .funcResetPos = &_resetPosBall
        };
    return result;
}

void shapeDraw(Shape *shape, float thick, Color c) {
    shape->funcDraw(shape, thick, c);
}

void shapeUpdate(Shape *shape) {
    shape->funcUpdate(shape);
}

void applyForce(Shape *shape, Vector2 force, float angForce) {
    shape->accel = Vec2Add(shape->accel, Vec2Div(force, shape->mass));
    shape->angAccel += angForce / shape->mass;
}

void applyFriction(Shape* shape) {
    float coefficient = 0.5f;
    Vector2 frictForce = Vec2Normalize(shape->velocity);
    frictForce = Vec2Scale(frictForce, coefficient * -1); // in Gegenrichtung
    frictForce = Vec2Limit(frictForce, Vec2Mag(shape->velocity));

    float frictAngDirection = shape->angVelocity < 0 ? 1 : -1; // in Gegenrichtung
    float frictAngForce = limitNum(coefficient * 0.05 * frictAngDirection, abs(shape->angVelocity));

    applyForce(shape, frictForce, frictAngForce);
}

void shapeResetPos(Shape *shape, Vector2 v) {
    shape->funcResetPos(shape, v);
}

Vector2 checkKick(Shape* shape) {

    Vector2 mousePos = { (float)GetMouseX(), (float)GetMouseY() };

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {

        if (Vec2Dist(shape->location, mousePos) < 10) {
            shape->marked = true;
        }

        if (shape->marked) {
            DrawLineEx(shape->location, mousePos, 3, RED); //draw arrow
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && shape->marked) {
        shape->marked = false;
        Vector2 force = Vec2Sub(mousePos, shape->location);
        force = Vec2Scale(force, 5);
        return force;
    }

    return (Vector2){0, 0};
}

CollisionPoint detectCollBox(Shape* boxA, Shape* boxB) {
    // Geprüft wird, ob eine Ecke von boxA in die Kante von boxB schneidet
    // Zusätzlich muss die Linie von Mittelpunkt boxA und Mittelpunkt boxB durch Kante von boxB gehen
    // i ist Index von Ecke und j ist Index von Kante
    // d = Diagonale von A.Mittelpunkt zu A.vertices(i)
    // e = Kante von B(j) zu B(j+1)
    // z = Linie von A.Mittelpunkt zu B.Mittelpunkt
    // _perp = Perpendicularvektor
    // scalar_d Faktor von d für den Schnittpunkt d/e
    // scalar_z Faktor von z für den Schnittpunkt z/e
    // mtv = minimal translation vector (überlappender Teil von d zur Kante e)

    for (int i = 0; i < 4; i++) {            
        for (int j = 0; j < 4; j++) {
            // Prüfung auf intersection von Diagonale d zu Kante e
            Intersection isd = intersectLine(boxA->location, boxA->vertices[i], boxB->vertices[j], boxB->vertices[j + 1]);   
            if (isd.distance > 0.0f) {
                // Prüfung auf intersection Linie z zu Kante e
                Intersection isz = intersectLine(boxA->location, boxB->location, boxB->vertices[j], boxB->vertices[j + 1]);
                if (isz.distance > 0.0f) {
                    // Collision findet statt
                    // Objekte zurücksetzen und normal_e berechnen. Kollisionspunkt ist Ecke i von BoxA
                    Vector2 e = Vec2Sub(boxB->vertices[j + 1], boxB->vertices[j]);
                    Vector2 e_perp = {-(e.y), e.x};
                    Vector2 d = Vec2Sub(boxA->vertices[i], boxA->location);  
                    d = Vec2Scale(d, 1-isd.distance);
                    e_perp = Vec2Normalize(e_perp);
                    float distance = Vec2DotProduct(e_perp, d);
                    e_perp = Vec2Scale(e_perp, -distance); //mtv
                    shapeResetPos(boxA, Vec2Scale(e_perp, 0.5f));
                    shapeResetPos(boxB, Vec2Scale(e_perp, -0.5f));
                    e_perp = Vec2Normalize(e_perp); // normal_e
                    return (CollisionPoint){true, boxA->vertices[i], e_perp};                }
            }
        }
    }
    return (CollisionPoint) {false, {0.0f, 0.0f}, {0.0f, 0.0f}};
};

CollisionPoint detectCollBall(Shape* ballA, Shape* ballB) {
    //Distanz ermitteln
    float radiusTotal = ballA->radius + ballB->radius;
    float dist = Vec2Dist(ballA->location, ballB->location);
    if (dist < radiusTotal) {
        //Treffer
        float space = (radiusTotal - dist);
        Vector2 collisionLine = Vec2Sub(ballA->location, ballB->location);
        collisionLine = Vec2SetMag(collisionLine, space);
        shapeResetPos(ballA, Vec2Scale( collisionLine, 0.5));
        shapeResetPos(ballB, Vec2Scale( collisionLine, -0.5));
        collisionLine = Vec2Normalize(collisionLine);
        return (CollisionPoint) {true, {0.0f, 0.0f}, collisionLine};
    }
    return (CollisionPoint) {false, {0.0f, 0.0f}, {0.0f, 0.0f}};
}

CollisionPoint detectCollBallBox(Shape* ball, Shape* box) {
    for (int i = 0; i < 4; i++) {
        //Kante der Box
        Vector2 e = Vec2Sub(box->vertices[i+1], box->vertices[i]);
        //Vektor von Ecke der Box zum Ball
        Vector2 VerticeToBall = Vec2Sub(ball->location, box->vertices[i]);
        //Kollision mit Ecken abfangen
        if (Vec2Mag(VerticeToBall) < ball->radius) {
            return (CollisionPoint){true, box->vertices[i], VerticeToBall};
        }
        float mag_e = Vec2Mag(e);
        e = Vec2Normalize(e);
        //Scalarprojektion von Vektor VerticeToBall auf Kante e
        float scalar_e = Vec2DotProduct(VerticeToBall, e);
        if (scalar_e > 0 && scalar_e <= mag_e) {
            //Senkrechte von Ball trifft auf Kante e der Box
            //e2 = Kante e mit der Länge von scalar_e
            Vector2 e2 = Vec2Scale(e, scalar_e);
            //Senkrechte von e zum Ball = VerticeToBall - e2
            Vector2 e_perp = Vec2Sub(VerticeToBall, e2);

            if (Vec2Mag(e_perp) < ball->radius) {
                //Ball berührt Box
                //Abstand wieder herstellen mit mtv (minimal translation vector)
                Vector2 mtv = e_perp;
                Vector2 p = Vec2Add(box->vertices[i], e2);
                mtv = Vec2SetMag(mtv, ball->radius - Vec2Mag(e_perp));
                //e_perp und damit mtv zeigt von Kante zu Ball
                shapeResetPos(ball, mtv);
                //vor Berechnung muss e_perp normalisiert werden
                e_perp = Vec2Normalize(e_perp);
                //resolveCollisionBallBox(ball, box, p, e_perp)
                return (CollisionPoint){true, p, e_perp};
            }
        }
    }
    return (CollisionPoint) {false, {0.0f, 0.0f}, {0.0f, 0.0f}};
}

void resolveCollBox(Shape* boxA, Shape* boxB, Vector2 cp, Vector2 normal) {
    // rAP = Linie von A.location zu Kollisionspunkt (Ecke i von BoxA)
    Vector2 rAP = Vec2Sub(cp, boxA->location);
    // rBP = Linie von B.location zu Kollisionspunkt (ebenfalls Ecke i von BoxA)
    Vector2 rBP = Vec2Sub(cp, boxB->location);
    Vector2 rAP_perp = {-rAP.y, rAP.x};
    Vector2 rBP_perp = {-rBP.y, rBP.x};
    Vector2 VtanA = Vec2Scale(rAP_perp, boxA->angVelocity);
    Vector2 VtanB = Vec2Scale(rBP_perp, boxB->angVelocity);
    Vector2 VgesamtA = Vec2Add(boxA->velocity, VtanA);
    Vector2 VgesamtB = Vec2Add(boxB->velocity, VtanB);
    Vector2 velocity_AB = Vec2Sub(VgesamtA, VgesamtB);
    if (Vec2DotProduct(velocity_AB, normal) < 0) { // wenn negativ, dann auf Kollisionskurs
        float e = 1.0f; //inelastischer Stoß
        float j_denominator = Vec2DotProduct(Vec2Scale(velocity_AB, -(1+e)), normal);
        float j_divLinear = Vec2DotProduct(normal, Vec2Scale(normal, (1/boxA->mass + 1/boxB->mass)));
        float j_divAngular = (float)pow(Vec2DotProduct(rAP_perp, normal), 2) / boxA->inertia + (float)pow(Vec2DotProduct(rBP_perp, normal), 2) / boxB->inertia;
        float j = j_denominator / (j_divLinear + j_divAngular);
        // Grundlage für Friction berechnen (t)
        Vector2 t = {-(normal.y), normal.x};
        float t_scalarprodukt = Vec2DotProduct(velocity_AB, t);
        t = Vec2Scale(Vec2Normalize(t), (t_scalarprodukt));
        

        //apply Force        
        Vector2 force = Vec2Add(Vec2Scale(normal, (j/boxA->mass)), Vec2Scale(t, (0.2*-j/boxA->mass)));
        float force_ang = Vec2DotProduct(rAP_perp, Vec2Add(Vec2Scale(normal, j/boxA->inertia), Vec2Scale(t, 0.2*-j/boxA->inertia)));
        boxA->accel = Vec2Add(boxA->accel, force);
        boxA->angAccel += force_ang;

        force = Vec2Add(Vec2Scale(normal, (-j/boxB->mass)), Vec2Scale(t, (0.2*j/boxB->mass)));
        force_ang = Vec2DotProduct(rAP_perp, Vec2Add(Vec2Scale(normal, -j/boxB->inertia), Vec2Scale(t, 0.2*j/boxB->inertia)));
        boxB->accel = Vec2Add(boxB->accel, force);
        boxB->angAccel += force_ang;

    }
}

void resolveCollBall(Shape* ballA, Shape* ballB, Vector2 normal) {
    Vector2 rA = Vec2Scale(normal, -ballA->radius);
    Vector2 rA_perp = {-rA.y, rA.x};
    Vector2 rB = Vec2Scale(normal, ballB->radius);
    Vector2 rB_perp = {-rB.y, rB.x};
    Vector2 VtanA = Vec2Scale(rA_perp, ballA->angVelocity);
    Vector2 VtanB = Vec2Scale(rB_perp, ballB->angVelocity);
    Vector2 VgesamtA = Vec2Add(ballA->velocity, VtanA);
    Vector2 VgesamtB = Vec2Add(ballB->velocity, VtanB);
    Vector2 velocity_AB = Vec2Sub(VgesamtA, VgesamtB);   
    
    if (Vec2DotProduct(velocity_AB, normal) < 0) { // wenn negativ, dann auf Kollisionskurs
        float e = 1; //inelastischer Stoß
        float j_denominator = Vec2DotProduct(Vec2Scale(velocity_AB, -(1+e)), normal);
        float j_divLinear = Vec2DotProduct(normal, Vec2Scale(normal, (1/ballA->mass + 1/ballB->mass)));
        float j = j_denominator / j_divLinear;
        // Grundlage für Friction berechnen
        Vector2 t = {-normal.y, normal.x};
        float t_scalarprodukt = Vec2DotProduct(velocity_AB, t);
        t = Vec2Scale(Vec2Normalize(t), t_scalarprodukt);

        //apply Force
        Vector2 force = Vec2Add(Vec2Scale(normal, (0.8*j/ballA->mass)), Vec2Scale(t, (0.2*-j/ballA->mass)));
        float force_ang = Vec2DotProduct(rA_perp, Vec2Scale(t, 0.1*-j/ballA->inertia));
        ballA->accel = Vec2Add(ballA->accel, force);
        ballA->angAccel += force_ang;

        force = Vec2Add(Vec2Scale(normal, (0.8*-j/ballB->mass)), Vec2Scale(t, (0.2*j/ballB->mass)));
        force_ang = Vec2DotProduct(rB_perp, Vec2Scale(t, 0.1*j/ballB->inertia));
        ballB->accel = Vec2Add(ballB->accel, force);
        ballB->angAccel += force_ang;
    }
}

void resolveCollBallBox(Shape* ball, Shape* box, Vector2 cp, Vector2 normal) {
    Vector2 rA = Vec2Scale(normal, -ball->radius);
    Vector2 rA_perp = {-rA.y, rA.x};
    Vector2 rBP = Vec2Sub(cp, box->location);
    Vector2 rBP_perp = {-rBP.y, rBP.x};
    Vector2 VtanA = Vec2Scale(rA_perp, ball->angVelocity);
    Vector2 VgesamtA = Vec2Add(ball->velocity, VtanA);
    Vector2 VtanB = Vec2Scale(rBP_perp, box->angVelocity);
    Vector2 VgesamtB = Vec2Add(box->velocity, VtanB);
    Vector2 velocity_AB = Vec2Sub(VgesamtA, VgesamtB);

    if (Vec2DotProduct(velocity_AB, normal) < 0) { // wenn negativ, dann auf Kollisionskurs

        float e = 1; //inelastischer Stoß
        float j_denominator = Vec2DotProduct(Vec2Scale(velocity_AB, -(1+e)), normal);
        float j_divLinear = Vec2DotProduct(normal, Vec2Scale(normal, (1/ball->mass + 1/box->mass)));
        float j_divAngular = (float)pow(Vec2DotProduct(rBP_perp, normal), 2) / box->inertia; //nur für Box zu rechnen
        float j = j_denominator / (j_divLinear + j_divAngular);
        // Grundlage für Friction berechnen
        Vector2 t = {-normal.y, normal.x};
        float t_scalarprodukt = Vec2DotProduct(velocity_AB, t);
        t = Vec2Scale(Vec2Normalize(t), t_scalarprodukt);

        // Apply Force
        Vector2 force = Vec2Add(Vec2Scale(normal, (0.8*j/ball->mass)), Vec2Scale(t, (0.05*-j/ball->mass)));
        float force_ang = Vec2DotProduct(rA_perp, Vec2Scale(t, 0.05*-j/ball->inertia));
        ball->accel = Vec2Add(ball->accel, force);
        ball->angAccel += force_ang;
           
        force = Vec2Add(Vec2Scale(normal, (-j/box->mass)), Vec2Scale(t, (0.05*j/box->mass)));
        force_ang = Vec2DotProduct(rBP_perp, Vec2Add(Vec2Scale(normal, -j/box->inertia), Vec2Scale(t, 0.05*j/box->inertia)));
        box->accel = Vec2Add(box->accel, force);
        box->angAccel += force_ang;
    }
}

void checkColl(Shape* shapeA, Shape* shapeB) {
    //Shadow berechnen von Element i und Element j 
    Shadow shadowA = createShadow(shapeA);
    Shadow shadowB = createShadow(shapeB);
    //Überschneidung prüfen
    if (shadowA.maxX >= shadowB.minX && shadowA.minX <= shadowB.maxX && shadowA.maxY >= shadowB.minY && shadowA.minY <= shadowB.maxY) {  
        //dann Überschneidung
        // Testcode
        DrawLineV(shapeA->location, shapeB->location, GREEN);
        // Ende Testcodew

        if (shapeA->typ == BALL) {
            if (shapeB->typ == BALL) {
                CollisionPoint cp = detectCollBall(shapeA, shapeB);
                if (cp.isCollision) {
                    resolveCollBall(shapeA, shapeB, cp.normal);
                }
            } else {
                CollisionPoint cp = detectCollBallBox(shapeA, shapeB);
                if (cp.isCollision) {
                    resolveCollBallBox(shapeA, shapeB, cp.cp, cp.normal);
                }
            }
        }

        if (shapeA->typ == BOX) {
            if (shapeB->typ == BOX) {
                // beide Boxen müssen geprüft werden, ob sie auf
                // die jeweils andere trefen könnte
                CollisionPoint cp = detectCollBox(shapeA, shapeB);
                if (cp.isCollision) {
                    resolveCollBox(shapeA, shapeB, cp.cp, cp.normal);  
                } else {
                    CollisionPoint cp = detectCollBox(shapeA, shapeB);    
                    if (cp.isCollision) {
                        resolveCollBox(shapeA, shapeB, cp.cp, cp.normal);
                    }
                }
            } else {
                CollisionPoint cp = detectCollBallBox(shapeB, shapeA);
                if (cp.isCollision) {
                    resolveCollBallBox(shapeB, shapeA, cp.cp, cp.normal);
                }
            }            
        }
    }
}