#include "gl/glut.h"
#include <cstdio>
#include <algorithm>
#include <cmath>

struct Vec2 {
    float x, y;
};

Vec2 linePt[6] = {
    {-0.3f, 0.2f},
    {0.6f, -0.7f},
    {-0.7f, -0.5f},
    {0.5f, 0.0f}
};

const float EPSILON = 1e-5; // �ε��Ҽ��� �񱳸� ���� ���� ��

// �� ���� ���� ������ ���ϴ� �Լ�
bool almostEqual(float a, float b) {
    return std::abs(a - b) < EPSILON;
}

bool isBetween(float value, float end1, float end2) {
    // value�� end1�� end2 ���̿� �ִ��� Ȯ�� (end1�� end2���� Ŭ ���� �����Ƿ�)
    return (std::min(end1, end2) <= value && value <= std::max(end1, end2));
}

// ������ ��� �Լ�
Vec2 calculateIntersection(Vec2 a1, Vec2 a2, Vec2 b1, Vec2 b2) {
    float denominator = (a1.x - a2.x) * (b1.y - b2.y) - (a1.y - a2.y) * (b1.x - b2.x);

    // ���� �Ǵ� ��ġ�ϴ� ��� -3 ��ȯ
    if (almostEqual(denominator, 0.0)) {
        // ������ ��ǥ�� ������ �������� �߰� �˻�
        if ((almostEqual(a1.x, b1.x) && almostEqual(a1.y, b1.y) &&
            almostEqual(a2.x, b2.x) && almostEqual(a2.y, b2.y)) ||
            (almostEqual(a1.x, b2.x) && almostEqual(a1.y, b2.y) &&
                almostEqual(a2.x, b1.x) && almostEqual(a2.y, b1.y))) {
            return Vec2{ -2, -2 }; // ������ ��ġ
        }
        return Vec2{ -3, -3 }; // ������ ����
    }

    float intersectionX = ((a1.x * a2.y - a1.y * a2.x) * (b1.x - b2.x) - (a1.x - a2.x) * (b1.x * b2.y - b1.y * b2.x)) / denominator;
    float intersectionY = ((a1.x * a2.y - a1.y * a2.x) * (b1.y - b2.y) - (a1.y - a2.y) * (b1.x * b2.y - b1.y * b2.x)) / denominator;

    // �������� ���� ���� ���� ���� ��� -1 ��ȯ
    if (!(isBetween(intersectionX, a1.x, a2.x) && isBetween(intersectionY, a1.y, a2.y) &&
        isBetween(intersectionX, b1.x, b2.x) && isBetween(intersectionY, b1.y, b2.y))) {
        return Vec2{ -1, -1 }; // ������ �������� ����
    }

    return Vec2{ intersectionX, intersectionY }; // ��ȿ�� ������
}
// �ؽ�Ʈ�� ȭ�鿡 �������ϴ� �Լ�
void renderBitmapString(float x, float y, void* font, const char* string) {
    const char* c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

void display() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex2f(linePt[0].x, linePt[0].y);
    glVertex2f(linePt[1].x, linePt[1].y);
    glEnd();

    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex2f(linePt[2].x, linePt[2].y);
    glVertex2f(linePt[3].x, linePt[3].y);
    glEnd();

    /* Calculate the interseciton point! */
    Vec2 intersection = calculateIntersection(linePt[0], linePt[1], linePt[2], linePt[3]);
    // ��ȿ�� �������� ǥ��
    if (intersection.x != -1 && intersection.y != -1) {
        glColor3f(0.0, 1.0, 0.0); // ���
        glPointSize(10.0);
        glBegin(GL_POINTS);
        glVertex2f(intersection.x, intersection.y);
        glEnd();
    }
    else {
        // ������ ũ���� �߾� ���� ����ϴ�.
        int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
        int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

        // ���� �޽����� �����ϰ� �߾ӿ� �������մϴ�.
        glColor3f(1.0, 0.0, 0.0); // ���������� ����
        if (intersection.x == -2 && intersection.y == -2) {
            renderBitmapString(0.0f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Lines Coincide");
        }
        else if (intersection.x == -3 && intersection.y == -3) {
            renderBitmapString(0.0f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "Parallel Lines");
        }
        else {
            renderBitmapString(0.0f, 0.0f, GLUT_BITMAP_TIMES_ROMAN_24, "No Intersection");
        }
    }
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) { 

    switch (key) {
	case 'w':
	    linePt[0].y += 0.1f;
	    linePt[1].y += 0.1f;
	    break;
	case 's':
	    linePt[0].y -= 0.1f;
	    linePt[1].y -= 0.1f;
	    break;
	case 'a':
	    linePt[0].x -= 0.1f;
	    linePt[1].x -= 0.1f;
	    break;
	case 'd':
	    linePt[0].x += 0.1f;
	    linePt[1].x += 0.1f;
	    break;
	case 27: // ESC
	    exit(0);
	    break;
    }
    glutPostRedisplay();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(1480, 100);


    glutCreateWindow("OpenGL");
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();

    return 0;
}