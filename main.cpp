
#include <iostream>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>


int FlatShaded = 0;
int Wireframed = 0;
int ViewX = 0, ViewY = 0;
float angleX = 0.0f;
float angleY = 0.0f;
int prevMouseX = 0, prevMouseY = 0;
void InitLight() {
    GLfloat mat_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
    GLfloat mat_shininess[] = { 15.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat light_position[] = { -3.0, 6.0, 3.0, 0.0 };
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void DrawTable() {
    glPushMatrix();
    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(0.0, -0.3, 0.0);  // 상판 높이 조절
    glScalef(0.5, 0.05, 0.5);  // 상판 크기 조절
    glutSolidCube(1.0);  // 상판 큐브 그리기
    glPopMatrix();

    float legHeight = 0.25f;
    float legThickness = 0.05f;
    float offset = 0.225f;

    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            glPushMatrix();
            glColor3f(0.9f, 0.9f, 0.9f);
            glTranslatef(i * offset, -0.3f - legHeight / 2, j * offset);
            glScalef(legThickness, legHeight, legThickness);
            glutSolidCube(1.0);
            glPopMatrix();
        }
    }
}

void DrawTeapotAndCups() {
    // 찻주전자 그리기 (옅은 하얀색)
    glPushMatrix();
    glColor3f(0.9f, 0.9f, 0.9f);
    glTranslatef(0.0, -0.2, 0.0);  // 찻주전자를 탁자 상판 위에 배치
    glutSolidTeapot(0.1);
    glPopMatrix();

    // 찻잔 그리기 (옅은 하얀색)
    glPushMatrix();
    glColor3f(0.9f, 0.9f, 0.9f);  // 옅은 하얀색
    glTranslatef(0.15, -0.25, 0.1);  // 찻잔 위치 조정
    glutSolidSphere(0.05, 20, 20);  // 구 형태의 찻잔
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.15, -0.25, -0.1);  // 두 번째 찻잔 위치 조정
    glutSolidSphere(0.05, 20, 20);  // 구 형태의 찻잔
    glPopMatrix();
}

void MyMouseMove(int x, int y) {
    ViewX = x;
    ViewY = y;
    glutPostRedisplay();
}

void MyKeyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'q': case 'Q': case '\033':  // Esc 키
            exit(0);
            break;
        case 's':
            FlatShaded = !FlatShaded;
            glShadeModel(FlatShaded ? GL_FLAT : GL_SMOOTH);
            glutPostRedisplay();
            break;
        case 'w':
            Wireframed = !Wireframed;
            glPolygonMode(GL_FRONT_AND_BACK, Wireframed ? GL_LINE : GL_FILL);
            glutPostRedisplay();
            break;
    }
}

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 카메라 시점 설정: 마우스 움직임에 따라 변경
    float eyeX = sin(angleX * M_PI / 180.0) * 1.5;
    float eyeY = angleY * 0.01;
    float eyeZ = cos(angleX * M_PI / 180.0) * 1.5;

    gluLookAt(eyeX, 0.5 + eyeY, eyeZ, 0.0, -0.2, 0.0, 0.0, 1.0, 0.0);

    // 탁자와 찻주전자, 찻잔 그리기
    DrawTable();
    DrawTeapotAndCups();

    glFlush();
}

void MyReshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 10.0);
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Sample Drwaing");

    glClearColor(0.4, 0.4, 0.4, 0.0);
    InitLight();
    glutDisplayFunc(MyDisplay);
    glutKeyboardFunc(MyKeyboard);
    glutMotionFunc(MyMouseMove);
    glutReshapeFunc(MyReshape);
    glutMainLoop();
    return 0;
}

