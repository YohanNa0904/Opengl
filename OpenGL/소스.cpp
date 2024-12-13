#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// 전역 변수
GLfloat cameraX = 0.0f, cameraY = 1.5f, cameraZ = 10.0f; // 카메라 위치
GLfloat cameraAngle = 0.0f; // 카메라 방향 각도 (Y축 기준)
GLfloat cameraPitch = 0.0f; // 카메라 위/아래 각도
GLfloat treeAngle = 0.0f; // 나뭇잎 흔들림 각도
GLuint texture;  // 텍스쳐를 저장할 변수

// 햇빛 색상
GLfloat lightAmbientMorning[] = { 1.0f, 0.9f, 0.6f, 1.0f }; // 아침
GLfloat lightDiffuseMorning[] = { 1.0f, 0.8f, 0.5f, 1.0f };
GLfloat lightAmbientSunset[] = { 1.0f, 0.4f, 0.1f, 1.0f };  // 석양
GLfloat lightDiffuseSunset[] = { 1.0f, 0.3f, 0.0f, 1.0f };

// 현재 상태 (아침 / 석양)
int isMorning = 1;

// 마우스 상태 변수
int mouseLeftDown = 0;
int lastMouseX, lastMouseY;

// 함수 선언
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void reshape(int w, int h);
void drawGround();
void drawTree(float x, float z);
void drawBench(float x, float z, float angle);
void drawLampPost(float x, float z);
void drawFlower(float x, float z);
void drawFence(float x, float z);

// 초기화 함수
void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    glClearColor(0.5f, 0.7f, 1.0f, 1.0f);

    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
}

void loadTexture(const char* imagePath) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(imagePath, &width, &height, &nrChannels, 0);
    if (data) {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // 텍스쳐 설정
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 이미지 텍스쳐로 로드
        if (nrChannels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else if (nrChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);  // 메모리 해제
    }
    else {
        printf("Failed to load texture\n");
        stbi_image_free(data);
    }
}

// 바닥 및 길 그리기
void drawGround() {
    glPushMatrix();
    glColor3f(0.2f, 0.8f, 0.2f);
    glTranslatef(0.0f, -0.1f, 0.0f);
    glScalef(30.0f, 0.1f, 30.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.5f, 0.35f, 0.2f);
    glTranslatef(0.0f, -0.05f, 0.0f);
    glScalef(4.0f, 0.1f, 30.0f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// 나무 그리기
void drawTree(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(treeAngle, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glColor3f(0.55f, 0.27f, 0.07f);
    glTranslatef(0.0f, 0.75f, 0.0f);
    glScalef(0.2f, 1.5f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0f, 0.5f, 0.0f);
    for (float offsetY = 1.5f; offsetY <= 3.0f; offsetY += 0.5f) {
        glPushMatrix();
        glTranslatef(0.0f, offsetY, 0.0f);
        glutSolidSphere(0.8f - (offsetY - 1.5f) * 0.2f, 20, 20);
        glPopMatrix();
    }
    glPopMatrix();

    glPopMatrix();
}

// 벤치 그리기
void drawBench(float x, float z, float angle) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    // 벤치 좌석
    glPushMatrix();
    glColor3f(0.6f, 0.3f, 0.1f);
    glTranslatef(0.0f, 0.2f, 0.0f);
    glScalef(1.5f, 0.1f, 0.5f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // 벤치 다리 (4개)
    glPushMatrix();
    glColor3f(0.3f, 0.2f, 0.1f);

    // 앞쪽 왼쪽 다리
    glPushMatrix();
    glTranslatef(-0.6f, 0.1f, 0.2f);
    glScalef(0.1f, 0.2f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // 앞쪽 오른쪽 다리
    glPushMatrix();
    glTranslatef(0.6f, 0.1f, 0.2f);
    glScalef(0.1f, 0.2f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // 뒤쪽 왼쪽 다리
    glPushMatrix();
    glTranslatef(-0.6f, 0.1f, -0.2f);
    glScalef(0.1f, 0.2f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // 뒤쪽 오른쪽 다리
    glPushMatrix();
    glTranslatef(0.6f, 0.1f, -0.2f);
    glScalef(0.1f, 0.2f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();
}

// 가로등 그리기
void drawLampPost(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);

    // 기둥
    glPushMatrix();
    glColor3f(0.3f, 0.3f, 0.3f);
    glTranslatef(0.0f, 1.0f, 0.0f);
    glScalef(0.1f, 2.0f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // 조명
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.5f); // 밝은 노란색
    glTranslatef(0.0f, 2.2f, 0.0f);
    glutSolidSphere(0.2f, 20, 20);
    glPopMatrix();

    glPopMatrix();
}
// 꽃 그리기
void drawFlower(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);

    // 꽃 줄기
    glPushMatrix();
    glColor3f(0.0f, 0.8f, 0.0f);
    glTranslatef(0.0f, 0.2f, 0.0f);
    glScalef(0.05f, 0.4f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // 꽃잎
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.5f, 0.0f);
    glutSolidSphere(0.1f, 20, 20);
    glPopMatrix();

    glPopMatrix();
}

// 팬스 그리기 (둘레를 따라 배치)
void drawFence(float x, float z, bool isVertical) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);

    if (isVertical) {
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // 좌/우 측 팬스는 90도 회전
    }

    // 팬스 기둥
    glPushMatrix();
    glColor3f(0.5f, 0.5f, 0.5f);  // 회색
    glTranslatef(0.0f, 0.5f, 0.0f);
    glScalef(0.1f, 1.0f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // 팬스 가로 막대기
    glPushMatrix();
    glColor3f(0.5f, 0.5f, 0.5f);  // 회색
    glTranslatef(0.0f, 0.8f, 0.0f);
    glScalef(3.0f, 0.1f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();
}

// 표지판 그리기
void drawSign(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);

    // 기둥
    glPushMatrix();
    glColor3f(0.6f, 0.3f, 0.1f); // 갈색 기둥 색상
    glTranslatef(0.0f, 0.25f, 0.0f); // 기둥 위치 조정 (기둥 높이 절반)
    glScalef(0.1f, 0.5f, 0.1f); // 기둥 크기 (y축 길이를 절반으로)
    glutSolidCube(1.0f);
    glPopMatrix();

    // 표지판 판넬 (직사각형)
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);  // 표지판 판넬 색상 (흰색)

    glTranslatef(0.0f, 0.4f, 0.0f);  // 판넬 위치
    glScalef(1.5f, 1.0f, 0.05f);     // 판넬 크기
    // 텍스쳐 매핑
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.75f, 0.0f, -0.025f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.75f, 0.0f, -0.025f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.75f, 1.0f, -0.025f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.75f, 1.0f, -0.025f);
    glEnd();

    glPopMatrix();

    glPopMatrix();
}

void toggleLighting() {
    if (isMorning) {
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbientSunset);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuseSunset);
    }
    else {
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbientMorning);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuseMorning);
    }
}

// 디스플레이 함수
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    GLfloat lookX = cameraX + cos(cameraPitch) * sin(cameraAngle);
    GLfloat lookY = cameraY + sin(cameraPitch);
    GLfloat lookZ = cameraZ - cos(cameraPitch) * cos(cameraAngle);
    gluLookAt(cameraX, cameraY, cameraZ, lookX, lookY, lookZ, 0.0f, 1.0f, 0.0f);

    drawGround();

    // 나무 및 벤치 배치
    for (float x = -12.0f; x <= 12.0f; x += 6.0f) {
        for (float z = -12.0f; z <= 12.0f; z += 6.0f) {
            if (fabs(x) > 2.0f) {
                drawTree(x, z); // 나무
            }
        }
    }

    // 벤치 배치
    drawBench(-5.0f, 5.0f, 90.0f);  // 왼쪽 길 옆
    drawBench(5.0f, -5.0f, -90.0f); // 오른쪽 길 옆
    drawBench(5.0f, 10.0f, -90.0f); // 오른쪽 상단 길 옆
    drawBench(-5.0f, -10.0f, 90.0f); // 왼쪽 하단 길 옆

    // 가로등 배치
    drawLampPost(-2.0f, 2.0f); // 왼쪽 길
    drawLampPost(2.0f, -2.0f); // 오른쪽 길
    drawLampPost(-2.0f, -8.0f); // 하단 길
    drawLampPost(2.0f, 8.0f);  // 상단 길

    // 꽃 배치
    for (float x = -15.0f; x <= 15.0f; x += 6.0f) {
        for (float z = -12.0f; z <= 12.0f; z += 6.0f) {
            if (fabs(x) > 2.0f) {
                drawFlower(x, z); // 꽃
            }
        }
    }

    // 팬스 배치 (공원의 네 모서리)
    for (float x = -15.0f; x <= 15.0f; x += 3.0f) {
        drawFence(x, 15.0f, false);  // 상단 변
        drawFence(x, -15.0f, false); // 하단 변
    }
    for (float z = -15.0f; z <= 15.0f; z += 3.0f) {
        drawFence(15.0f, z, true);  // 오른쪽 변 (90도 회전)
        drawFence(-15.0f, z, true); // 왼쪽 변 (90도 회전)
    }

    // 표지판 배치
    drawSign(0.0f, 0.0f);  // 길 중앙에 표지판 배치

    toggleLighting(); // 햇빛 색상 전환

    glutSwapBuffers();
}

void loadTextures() {
    loadTexture("C:/Users/YohanNa/source/repos/Project5/A.png");  // 텍스쳐 경로 설정
}


// 키보드 입력 처리
void keyboard(unsigned char key, int x, int y) {
    GLfloat speed = 0.5f;
    switch (key) {
    case 'f':
        isMorning = !isMorning; // 'f' 키로 아침/석양 전환
        glutPostRedisplay();
    case 'w':
        cameraX += sin(cameraAngle) * speed;
        cameraZ -= cos(cameraAngle) * speed;
        break;
    case 's':
        cameraX -= sin(cameraAngle) * speed;
        cameraZ += cos(cameraAngle) * speed;
        break;
    case 'a':
        cameraX -= cos(cameraAngle) * speed;
        cameraZ -= sin(cameraAngle) * speed;
        break;
    case 'd':
        cameraX += cos(cameraAngle) * speed;
        cameraZ += sin(cameraAngle) * speed;
        break;
    case 27:
        exit(0);
        break;
    }
    glutPostRedisplay();
}

// 마우스 클릭 이벤트 처리
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            mouseLeftDown = 1;
            lastMouseX = x;
            lastMouseY = y;
        }
        else if (state == GLUT_UP) {
            mouseLeftDown = 0;
        }
    }
}

// 마우스 이동 이벤트 처리
void motion(int x, int y) {
    if (mouseLeftDown) {
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;

        cameraAngle += dx * 0.005f;
        cameraPitch -= dy * 0.005f;

        if (cameraPitch > 1.5f) cameraPitch = 1.5f;
        if (cameraPitch < -1.5f) cameraPitch = -1.5f;

        lastMouseX = x;
        lastMouseY = y;

        glutPostRedisplay();
    }
}

// 리쉐이프 함수
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// 메인 함수
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Park Scene with Lamp Posts");

    init();
    loadTextures();  // 텍스쳐 로드

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();
    return 0;
}