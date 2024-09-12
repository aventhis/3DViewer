#include "s21_openglwidget.h"

s21_OpenGLWidget::s21_OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

void s21_OpenGLWidget::initializeGL() {
  scale = 1.0;
  glEnable(GL_DEPTH_TEST);
}

void s21_OpenGLWidget::paintGL() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(s21_background_openjl.red, s21_background_openjl.green,
               s21_background_openjl.blue, 1.0f);
  if (status != 0 || data_f.values == nullptr || data_v.values == nullptr) {
    return;
  }
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (projection == PARALLEL) {
    glOrtho(-1.0, 1.0, -1.0, 1.0, 2, 15);
  } else {
    glFrustum(-1, 1, -1, 1, 2, 15);
  }
  glTranslated(0, 0, -3);
  // Apply rotation transformations
  glRotatef(rotationX, 1.0f, 0.0f, 0.0f);
  glRotatef(rotationY, 0.0f, 1.0f, 0.0f);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, data_v.values);

  if (vertextype != VERTEX_NONE) {
    glColor3f(s21_vertex.red, s21_vertex.green, s21_vertex.blue);
    glPointSize(vertex_thickness);
    if (vertextype == VERTEX_CIRCLE) {
      glEnable(GL_POINT_SMOOTH);
    } else {
      glDisable(GL_POINT_SMOOTH);
    }
    glDrawArrays(GL_POINTS, 0, data_v.count / 3);
  }
  //  // цвет линий
  glColor3f(s21_line.red, s21_line.green, s21_line.blue);

  if (linetype == LINE_DOTTED) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);
  }
  //  // толщина линий
  glLineWidth(line_thickness);

  glDrawElements(GL_LINES, data_f.count, GL_UNSIGNED_INT, data_f.values);
  if (linetype == LINE_DOTTED) glDisable(GL_LINE_STIPPLE);

  glDisableClientState(GL_VERTEX_ARRAY);
}

void s21_OpenGLWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    isMousePressed = true;
    lastMousePos = event->pos();
  }
}

void s21_OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
  if (isMousePressed) {
    QPoint delta = event->pos() - lastMousePos;
    float sensitivity = 0.3;  // Чувствительность мыши для вращения
    rotationX += delta.y() * sensitivity;
    rotationY += delta.x() * sensitivity;

    // Обновляем виджет
    update();
    lastMousePos = event->pos();
  }
}

void s21_OpenGLWidget::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    isMousePressed = false;
  }
}
