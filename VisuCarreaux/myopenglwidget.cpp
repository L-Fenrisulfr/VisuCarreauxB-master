//R. Raffin, M1 Informatique, "Surfaces 3D"
//tiré de CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "myopenglwidget.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>

#include <iostream>


#include "segment.h"
#include "segments.h"

static const QString vertexShaderFile   = ":/basic.vsh";
static const QString fragmentShaderFile = ":/basic.fsh";

const int lvl_dis = 30;
myOpenGLWidget::myOpenGLWidget(QWidget *parent) :
	QOpenGLWidget(parent)
{
	qDebug() << "init myOpenGLWidget" ;

	QSurfaceFormat sf;
	sf.setDepthBufferSize(24);
	sf.setSamples(16);  // nb de sample par pixels : suréchantillonnage por l'antialiasing, en décalant à chaque fois le sommet
						// cf https://www.khronos.org/opengl/wiki/Multisampling et https://stackoverflow.com/a/14474260
	setFormat(sf);

	setEnabled(true);  // événements clavier et souris
	setFocusPolicy(Qt::StrongFocus); // accepte focus
	setFocus();                      // donne le focus

	m_timer = new QTimer(this);
	m_timer->setInterval(50);  // msec
    nb = new QLCDNumber(this);
	connect (m_timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

Point  OMT0[4];
Point  OMT1[4];
//-----------
// for(int j=1;j<n;j++)
    // for(int i=0;i<n-j,i++)
        // p[i][j]=t*p[i+1][j-1]+(1-t)*p[i][j-1]
float * casteljau()
{
    //P_0(-2,-2,0) P_1=(-1,1,0) P_2=(1,1,0) P_3=(2,-2,0)

    float * coord = new float[3];

    coord[0] = -2.0;
    coord[1] = -2.0f;
    coord[2] = 0.0f;

    Point *P0=new Point();
    P0->set(coord);

    coord[0] = -1.0;
    coord[1] = 1.0f;
    coord[2] = 0.0f;

    Point *P1=new Point();
    P1->set(coord);

    coord[0] = 1.0;
    coord[1] = 1.0f;
    coord[2] = 0.0f;

    Point *P2=new Point();
    P2->set(coord);

    coord[0] = 2.0;
    coord[1] = -2.0f;
    coord[2] = 0.0f;

    Point *P3=new Point();
    P3->set(coord);



    Point  OM0[4];
    Point  OM1[4];
    Point  OM2[4];

    for(int t=0;t<4;t++)
    {
        OM0[t].setX((1-t)*P0->getX()+t*P1->getX());
        OM1[t].setX((1-t)*P1->getX()+t*P2->getX());
        OM2[t].setX((1-t)*P2->getX()+t*P3->getX());

        OM0[t].setY((1-t)*P0->getY()+t*P1->getY());
        OM1[t].setY((1-t)*P1->getY()+t*P2->getY());
        OM2[t].setY((1-t)*P2->getY()+t*P3->getY());

        OM0[t].setZ((1-t)*P0->getZ()+t*P1->getZ());
        OM1[t].setZ((1-t)*P1->getZ()+t*P2->getZ());
        OM2[t].setZ((1-t)*P2->getZ()+t*P3->getZ());
        //
        /*
        OM0[t].y=(1-t)*P0->y+t*P1->y;
        OM1[t].y=(1-t)*P1->y+t*P2->y;
        OM2[t].y=(1-t)*P2->y+t*P3->y;

        OM0[t].z=(1-t)*P0->z+t*P1->z;
        OM1[t].z=(1-t)*P1->z+t*P2->z;
        OM2[t].z=(1-t)*P2->z+t*P3->z;
        */
    }
    float vertice[6*4];
    for(int t=0;t<4;t++)
    {
        for(int i=0;i<4;i++)
        {
            OMT0[t].setX((1-t)*OM0[i].getX()+t*OM1[i].getX());
            OMT1[t].setX((1-t)*OM1[i].getX()+t*OM2[i].getX());

            OMT0[t].setY((1-t)*OM0[i].getY()+t*OM1[i].getY());
            OMT1[t].setY((1-t)*OM1[i].getY()+t*OM2[i].getY());

            OMT0[t].setZ((1-t)*OM0[i].getZ()+t*OM1[i].getZ());
            OMT1[t].setZ((1-t)*OM1[i].getZ()+t*OM2[i].getZ());
            // cout<<"test1 : "<<OMT1[t]<<endl;
        }

    }
    int j=0;
    int i=0;
    while(j>3*4)
    {
        vertice[j]=OMT0[i].getX();
        j=j+1;
        vertice[j]=OMT0[i].getY();
        j=j+1;
        vertice[j]=OMT0[i].getZ();
        j=j+1;
        i=i+1;
    }
    int k=0;
    while(j>6*4)
    {
        qDebug()<<"test :"<<OMT1[k].getX()<<endl;
        vertice[j]=OMT1[k].getX();
        j=j+1;
        vertice[j]=OMT1[k].getY();
        j=j+1;
        vertice[j]=OMT1[k].getZ();
        j=j+1;
        k=k+1;
    }
    for(int i=0;i<4*6;i++)
        {
            qDebug()<<OMT0[i].getX()<<endl;
            qDebug()<<vertice[i];
        }
    return vertice;
    // glGenBuffers(1, &leVBO); //génération d'une référence de buffer object
    // glBindBuffer(GL_ARRAY_BUFFER, leVBO); //liaison du buffer avec un type tableau de données
    // glBufferData(GL_ARRAY_BUFFER, sizeof(float)*tailleTab, vertices, GL_STATIC_DRAW);
}
myOpenGLWidget::~myOpenGLWidget()
{
	qDebug() << "destroy GLArea";

	delete m_timer;

	// Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
	// dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
	makeCurrent();
	tearGLObjects();
	doneCurrent();
}




void myOpenGLWidget::initializeGL()
{
	qDebug() << __FUNCTION__ ;
	initializeOpenGLFunctions();
	glEnable(GL_DEPTH_TEST);

	makeGLObjects();

	//shaders
	m_program = new QOpenGLShaderProgram(this);
	m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexShaderFile);  // compile
	m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentShaderFile);

	if (! m_program->link()) {  // édition de lien des shaders dans le shader program
		qWarning("Failed to compile and link shader program:");
		qWarning() << m_program->log();
	}
}

void myOpenGLWidget::doProjection()
{
	//m_mod.setToIdentity();
	//modelMatrix.ortho( -aratio, aratio, -1.0f, 1.0f, -1.0f, 1.0f );
}


void myOpenGLWidget::makeGLObjects()
{
    //float * cas = casteljau();
    Segments tab;
	//1 Nos objets géométriques
	Point A, B;
    Point A2, B2;
	float * coord = new float[3];

	coord[0] = 0.0f;
	coord[1] = 0.0f;
	coord[2] = 0.0f;

	A.set (coord);

	coord[0] = 1.0f;
	coord[1] = 0.0f;
	coord[2] = 0.0f;

	B.set(coord);

	Segment S;
	S.setStart(A);
	S.setEnd(B);

    tab.add(S);
    //s2
    coord[0] = -0.5f;
    coord[1] = 0.0f;
    coord[2] = 0.0f;

    A2.set (coord);

    coord[0] = 0.5f;
    coord[1] = 0.0f;
    coord[2] = 0.0f;

    B2.set(coord);

    Segment S2;
    S2.setStart(A2);
    S2.setEnd(B2);

    tab.add(S2);

    coord[0] = -1.0f;
    coord[1] = 1.0f;
    coord[2] = 0.0f;

    A.set (coord);

    coord[0] = 1.0f;
    coord[1] = -1.0f;
    coord[2] = 0.0f;

    B.set(coord);

    Segment S3;
    S3.setStart(A);
    S3.setEnd(B);

    tab.add(S3);
    coord[0] = -1.5f;
    coord[1] = 1.0f;
    coord[2] = 0.0f;

    A.set (coord);

    coord[0] = 1.0f;
    coord[1] = -1.5f;
    coord[2] = 0.0f;

    B.set(coord);

    Segment S4;
    S4.setStart(A);
    S4.setEnd(B);
    tab.add(S4);

	delete [] coord;

	//qDebug() << "segment length " << S.length ();

	//2 Traduction en tableaux de floats
    //size = tab.size() * 2;
    QVector<Discretisation> dis;
    for(int m = 0; m < tab.size(); m++)
    {
        Discretisation d;
        d.set(lvl_dis,tab.get(m));
        d.calcCoord();
        dis.append(d);

    }
    qDebug()<<"size dis : "<<dis.size();
    Segments tab2;
    for(int m = 0; m < dis.size(); m++)
    {
        qDebug()<<"size dis : "<<dis.at(m).pts.size();
        for(int p = 0; p<dis.at(m).pts.size()-1;p = p+2)
        {

            Segment X;
            X.setStart(dis.at(m).pts.at(p));
            X.setEnd(dis.at(m).pts.at(p+1));
            tab2.add(X);
        }

    }
    qDebug()<<"size tab2 : "<<tab2.size();
    size = tab2.size();
    QVector<GLfloat> vertData;
    for(int l = 0; l < tab2.size(); l++)
    {
        GLfloat * vertices = new GLfloat[6*tab2.size()]; //2 sommets
        GLfloat * colors = new GLfloat[6*tab2.size()]; //1 couleur (RBG) par sommet
        Point begin, end;
        float * values = new float[3];

        //begin = S.getStart ();
        begin = tab2.get(l).getStart();
        begin.get(values);
        for (unsigned i=0; i<3; ++i)
            vertices[i] = values[i];

        end = tab2.get(l).getEnd ();
        end.get(values);
        for (unsigned i=0; i<3; ++i)
            vertices[3+i] = values[i];

        delete[] values;

        //couleur0 = rouge
        colors[0] = 1.0;
        colors[1] = 0.0;
        colors[2] = 0.0;

        //bleu
        colors[3] = 0.0;
        colors[4] = 0.0;
        colors[5] = 1.0;

        //3 spécialisation OpenGL

        for (int i = 0; i < 2; ++i) { //2 sommets
            // coordonnées sommets
            for (int j = 0; j < 3; j++) //3 coords par sommet
                vertData.append(vertices[i*3+j]);
            // couleurs sommets
            for (int j = 0; j < 3; j++) //1 RGB par sommet
                vertData.append(colors[i*3+j]);
        }

        //destruction des éléments de la phase 2
        delete [] vertices;
        delete [] colors;
    }

    qDebug()<<vertData.size();
	m_vbo.create();
	m_vbo.bind();

	//qDebug() << "vertData " << vertData.count () << " " << vertData.data ();
	m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
}


void myOpenGLWidget::tearGLObjects()
{
	m_vbo.destroy();
}


void myOpenGLWidget::resizeGL(int w, int h)
{
	qDebug() << __FUNCTION__ << w << h;

	//C'est fait par défaut
	glViewport(0, 0, w, h);

	m_ratio = (double) w / h;
	//doProjection();
}

void myOpenGLWidget::paintGL()
{
	qDebug() << __FUNCTION__ ;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_program->bind(); // active le shader program


	/// Ajout RR pour gérer les 3 matrices utiles
	/// à mettre dans doProjection() pour clarifier
	/// -----------------------------------------
		m_modelView.setToIdentity();
		m_modelView.lookAt(QVector3D(0.0f, 0.0f, 3.0f),    // Camera Position
						 QVector3D(0.0f, 0.0f, 0.0f),    // Point camera looks towards
						 QVector3D(0.0f, 1.0f, 0.0f));   // Up vector

		m_projection.setToIdentity ();
		m_projection.perspective(70.0, width() / height(), 0.1, 100.0); //ou m_ratio

		//m_model.translate(0, 0, -3.0);

		// Rotation de la scène pour l'animation
		m_model.rotate(m_angle, 0, 1, 0);

		QMatrix4x4 m = m_projection * m_modelView * m_model;
	///----------------------------

	m_program->setUniformValue("matrix", m);

	m_program->setAttributeBuffer("posAttr", GL_FLOAT, 0, 3, 6 * sizeof(GLfloat));
	m_program->setAttributeBuffer("colAttr", GL_FLOAT, 3 * sizeof(GLfloat), 3, 6 * sizeof(GLfloat));
	m_program->enableAttributeArray("posAttr");
	m_program->enableAttributeArray("colAttr");

	glPointSize (5.0f);
    glLineWidth(3.0);
    glDrawArrays(GL_POINTS, begin*lvl_dis/2, lvl_dis);
    glDrawArrays(GL_LINE_LOOP, begin*lvl_dis/2, lvl_dis);


    nb->display(begin*lvl_dis/2);
    nb->show();

	m_program->disableAttributeArray("posAttr");
	m_program->disableAttributeArray("colAttr");

	m_program->release();
}

void myOpenGLWidget::keyPressEvent(QKeyEvent *ev)
{
	qDebug() << __FUNCTION__ << ev->text();

	switch(ev->key()) {
		case Qt::Key_Z :
            m_angle += 0.4;
			if (m_angle >= 360) m_angle -= 360;
			update();
			break;
		case Qt::Key_A :
			if (m_timer->isActive())
				m_timer->stop();
			else m_timer->start();
			break;
		case Qt::Key_R :
			break;
    case Qt::Key_Right :
        if(begin + 2 < size)
            begin += 2;
        update();
        break;
    case Qt::Key_Left :
        if(begin-2>=0)
            begin -= 2 % size;
        update();
        break;
	}
}

void myOpenGLWidget::keyReleaseEvent(QKeyEvent *ev)
{
	qDebug() << __FUNCTION__ << ev->text();
}

void myOpenGLWidget::mousePressEvent(QMouseEvent *ev)
{
	qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void myOpenGLWidget::mouseReleaseEvent(QMouseEvent *ev)
{
	qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}

void myOpenGLWidget::mouseMoveEvent(QMouseEvent *ev)
{
	qDebug() << __FUNCTION__ << ev->x() << ev->y();
}

void myOpenGLWidget::onTimeout()
{
	qDebug() << __FUNCTION__ ;

	update();
}






