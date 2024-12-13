#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// ���� ����
GLfloat cameraX = 0.0f, cameraY = 1.5f, cameraZ = 10.0f; // ī�޶� ��ġ
GLfloat cameraAngle = 0.0f; // ī�޶� ���� ���� (Y�� ����)
GLfloat cameraPitch = 0.0f; // ī�޶� ��/�Ʒ� ����
GLfloat treeAngle = 0.0f; // ������ ��鸲 ����
GLuint texture;  // �ؽ��ĸ� ������ ����

// �޺� ����
GLfloat lightAmbientMorning[] = { 1.0f, 0.9f, 0.6f, 1.0f }; // ��ħ
GLfloat lightDiffuseMorning[] = { 1.0f, 0.8f, 0.5f, 1.0f };
GLfloat lightAmbientSunset[] = { 1.0f, 0.4f, 0.1f, 1.0f };  // ����
GLfloat lightDiffuseSunset[] = { 1.0f, 0.3f, 0.0f, 1.0f };

// ���� ���� (��ħ / ����)
int isMorning = 1;

// ���콺 ���� ����
int mouseLeftDown = 0;
int lastMouseX, lastMouseY;

// �Լ� ����
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

// �ʱ�ȭ �Լ�
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

        // �ؽ��� ����
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // �̹��� �ؽ��ķ� �ε�
        if (nrChannels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else if (nrChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);  // �޸� ����
    }
    else {
        printf("Failed to load texture\n");
        stbi_image_free(data);
    }
}

// �ٴ� �� �� �׸���
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

// ���� �׸���
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

// ��ġ �׸���
void drawBench(float x, float z, float angle) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);
    glRotatef(angle, 0.0f, 1.0f, 0.0f);

    // ��ġ �¼�
    glPushMatrix();
    glColor3f(0.6f, 0.3f, 0.1f);
    glTranslatef(0.0f, 0.2f, 0.0f);
    glScalef(1.5f, 0.1f, 0.5f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // ��ġ �ٸ� (4��)
    glPushMatrix();
    glColor3f(0.3f, 0.2f, 0.1f);

    // ���� ���� �ٸ�
    glPushMatrix();
    glTranslatef(-0.6f, 0.1f, 0.2f);
    glScalef(0.1f, 0.2f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // ���� ������ �ٸ�
    glPushMatrix();
    glTranslatef(0.6f, 0.1f, 0.2f);
    glScalef(0.1f, 0.2f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // ���� ���� �ٸ�
    glPushMatrix();
    glTranslatef(-0.6f, 0.1f, -0.2f);
    glScalef(0.1f, 0.2f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // ���� ������ �ٸ�
    glPushMatrix();
    glTranslatef(0.6f, 0.1f, -0.2f);
    glScalef(0.1f, 0.2f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();

    glPopMatrix();
}

// ���ε� �׸���
void drawLampPost(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);

    // ���
    glPushMatrix();
    glColor3f(0.3f, 0.3f, 0.3f);
    glTranslatef(0.0f, 1.0f, 0.0f);
    glScalef(0.1f, 2.0f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // ����
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 0.5f); // ���� �����
    glTranslatef(0.0f, 2.2f, 0.0f);
    glutSolidSphere(0.2f, 20, 20);
    glPopMatrix();

    glPopMatrix();
}
// �� �׸���
void drawFlower(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);

    // �� �ٱ�
    glPushMatrix();
    glColor3f(0.0f, 0.8f, 0.0f);
    glTranslatef(0.0f, 0.2f, 0.0f);
    glScalef(0.05f, 0.4f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // ����
    glPushMatrix();
    glColor3f(1.0f, 0.0f, 0.0f);
    glTranslatef(0.0f, 0.5f, 0.0f);
    glutSolidSphere(0.1f, 20, 20);
    glPopMatrix();

    glPopMatrix();
}

// �ҽ� �׸��� (�ѷ��� ���� ��ġ)
void drawFence(float x, float z, bool isVertical) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);

    if (isVertical) {
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f); // ��/�� �� �ҽ��� 90�� ȸ��
    }

    // �ҽ� ���
    glPushMatrix();
    glColor3f(0.5f, 0.5f, 0.5f);  // ȸ��
    glTranslatef(0.0f, 0.5f, 0.0f);
    glScalef(0.1f, 1.0f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // �ҽ� ���� �����
    glPushMatrix();
    glColor3f(0.5f, 0.5f, 0.5f);  // ȸ��
    glTranslatef(0.0f, 0.8f, 0.0f);
    glScalef(3.0f, 0.1f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    glPopMatrix();
}

// ǥ���� �׸���
void drawSign(float x, float z) {
    glPushMatrix();
    glTranslatef(x, 0.0f, z);

    // ���
    glPushMatrix();
    glColor3f(0.6f, 0.3f, 0.1f); // ���� ��� ����
    glTranslatef(0.0f, 0.25f, 0.0f); // ��� ��ġ ���� (��� ���� ����)
    glScalef(0.1f, 0.5f, 0.1f); // ��� ũ�� (y�� ���̸� ��������)
    glutSolidCube(1.0f);
    glPopMatrix();

    // ǥ���� �ǳ� (���簢��)
    glPushMatrix();
    glColor3f(1.0f, 1.0f, 1.0f);  // ǥ���� �ǳ� ���� (���)

    glTranslatef(0.0f, 0.4f, 0.0f);  // �ǳ� ��ġ
    glScalef(1.5f, 1.0f, 0.05f);     // �ǳ� ũ��
    // �ؽ��� ����
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

// ���÷��� �Լ�
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    GLfloat lookX = cameraX + cos(cameraPitch) * sin(cameraAngle);
    GLfloat lookY = cameraY + sin(cameraPitch);
    GLfloat lookZ = cameraZ - cos(cameraPitch) * cos(cameraAngle);
    gluLookAt(cameraX, cameraY, cameraZ, lookX, lookY, lookZ, 0.0f, 1.0f, 0.0f);

    drawGround();

    // ���� �� ��ġ ��ġ
    for (float x = -12.0f; x <= 12.0f; x += 6.0f) {
        for (float z = -12.0f; z <= 12.0f; z += 6.0f) {
            if (fabs(x) > 2.0f) {
                drawTree(x, z); // ����
            }
        }
    }

    // ��ġ ��ġ
    drawBench(-5.0f, 5.0f, 90.0f);  // ���� �� ��
    drawBench(5.0f, -5.0f, -90.0f); // ������ �� ��
    drawBench(5.0f, 10.0f, -90.0f); // ������ ��� �� ��
    drawBench(-5.0f, -10.0f, 90.0f); // ���� �ϴ� �� ��

    // ���ε� ��ġ
    drawLampPost(-2.0f, 2.0f); // ���� ��
    drawLampPost(2.0f, -2.0f); // ������ ��
    drawLampPost(-2.0f, -8.0f); // �ϴ� ��
    drawLampPost(2.0f, 8.0f);  // ��� ��

    // �� ��ġ
    for (float x = -15.0f; x <= 15.0f; x += 6.0f) {
        for (float z = -12.0f; z <= 12.0f; z += 6.0f) {
            if (fabs(x) > 2.0f) {
                drawFlower(x, z); // ��
            }
        }
    }

    // �ҽ� ��ġ (������ �� �𼭸�)
    for (float x = -15.0f; x <= 15.0f; x += 3.0f) {
        drawFence(x, 15.0f, false);  // ��� ��
        drawFence(x, -15.0f, false); // �ϴ� ��
    }
    for (float z = -15.0f; z <= 15.0f; z += 3.0f) {
        drawFence(15.0f, z, true);  // ������ �� (90�� ȸ��)
        drawFence(-15.0f, z, true); // ���� �� (90�� ȸ��)
    }

    // ǥ���� ��ġ
    drawSign(0.0f, 0.0f);  // �� �߾ӿ� ǥ���� ��ġ

    toggleLighting(); // �޺� ���� ��ȯ

    glutSwapBuffers();
}

void loadTextures() {
    loadTexture("C:/Users/YohanNa/source/repos/Project5/A.png");  // �ؽ��� ��� ����
}


// Ű���� �Է� ó��
void keyboard(unsigned char key, int x, int y) {
    GLfloat speed = 0.5f;
    switch (key) {
    case 'f':
        isMorning = !isMorning; // 'f' Ű�� ��ħ/���� ��ȯ
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

// ���콺 Ŭ�� �̺�Ʈ ó��
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

// ���콺 �̵� �̺�Ʈ ó��
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

// �������� �Լ�
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// ���� �Լ�
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Park Scene with Lamp Posts");

    init();
    loadTextures();  // �ؽ��� �ε�

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glutMainLoop();
    return 0;
}