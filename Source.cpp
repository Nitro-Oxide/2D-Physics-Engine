#include <iostream>
#include "raylib.h"
#include <vector>
#include <list>
#include <math.h>
class circle {
public:
	float posX, posY;
	float radius;
	bool freefall = false;
	Vector2 internal_velocity = { 10,10 };
	Vector2 Collision_Point = { posX, posY + radius };

	circle(float x, float y, float r) {
		posX = x;
		posY = y;
		radius = r;
	}
	//Physics of ball
		//Ball Races towards cursor
	void ball_follow(Vector2 target) {

		float x_distance = target.x - (*this).posX;
		float y_distance = target.y - (*this).posY;

		if (x_distance < 0) x_distance *= -1;
		if (y_distance < 0) y_distance *= -1;

		if (target.x != (*this).posX) {
			this->internal_velocity.x = x_distance;
			if (target.x > (*this).posX) (*this).posX += x_distance / 6;
			else (*this).posX -= x_distance / 6;
		}
		if (target.y != (*this).posY) {
			if (target.y > (*this).posY) (*this).posY += y_distance / 4;
			else (*this).posY -= y_distance / 4;
		}

	}
		//Manual control of Ball
	void ball_manual(Vector2 target) {
			posX = target.x;
			posY = target.y;
	}



	void ball_Velocity(Vector2 velocity) {
		posX += velocity.x;
		posY += velocity.y;
		Collision_Point = { posX,posY };

		if (this->posX >= 1000) this->posX = 0;
		if (this->posY >= 600) this->posY = 0;
	}
};



void DrawCircleCirc(circle circ, Color color) {
	DrawCircle(circ.posX, circ.posY, circ.radius, color);
}


using namespace std;
int main() {
	//Variables 
	circle ball(25, 25, 25);
	Vector2 MP;
	Vector2 velocity = {ball.internal_velocity.x,1 };
	Rectangle Ground = { 0,550,1000,600 };
	vector<Vector2> PathList;
	list<Vector2> PastPos;
	vector<Vector2>::iterator it;
	int counter = 0;
	
	InitWindow(1000, 600, "Physics Engine");
	while (!WindowShouldClose()) {
		ClearBackground(BLACK);
		SetTargetFPS(60);
		
		MP = GetMousePosition();
		PastPos.push_front(MP);
		if (PastPos.size() > 2) {
			PastPos.pop_back();
		}

		//Ball Physics
		if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
			ball.ball_follow(MP);
			velocity.y = 0;
		}
		else if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			ball.ball_manual(MP);
			velocity.x = PastPos.front().x - PastPos.back().x;
			velocity.y = PastPos.front().y - PastPos.back().y;
		}
		else {
			velocity.x = ball.internal_velocity.x;
			ball.ball_Velocity(velocity);
			if (velocity.y < 20) {
				velocity.y += 1;
			}
		}

		if (CheckCollisionPointRec(ball.Collision_Point,Ground)) {
			velocity.y = -1 * velocity.y;
			counter++;
		}
		Vector2 ballHist = { ball.posX, ball.posY };
		PathList.push_back(ballHist);
		
		/*BALL COMMANDS*/
		//Ball reset
		if (IsKeyPressed(KEY_R)) {
			ball.posX = 25;
			ball.posY = 25;
			PathList.clear();
		}
		//Drawing 
		BeginDrawing();
		DrawText(TextFormat("Mouse Position: (%f,%f)", MP.x, MP.y), 350, 0, 15, WHITE);
		DrawText(TextFormat("Ball Position: (%f,%f)", ball.posX, ball.posY), 350, 20, 15, WHITE);
		DrawText(TextFormat("velocity: %f", velocity.x), 350, 30, 15, WHITE);
		DrawText(TextFormat("Collision point: (%f,%f)", ball.Collision_Point.x,ball.Collision_Point.y), 350, 45, 15, WHITE);
		DrawCircleCirc(ball, GREEN);
		DrawRectangleRec(Ground, GRAY);
		for (it = PathList.begin(); it != PathList.end(); it++) {
			DrawPixel(it->x, it->y, WHITE);
		}
		EndDrawing();
	}
	CloseWindow();
	return 0;

}