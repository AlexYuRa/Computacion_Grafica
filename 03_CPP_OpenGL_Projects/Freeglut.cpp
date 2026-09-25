#include <GL/glut.h>

// float angulo = 0;        // Ángulo de rotación
float tx = 0;            // Posición en el eje X
float ty = 0;            // Posición en el eje Y (para el salto)
int saltando = 0;        // Bandera para controlar el salto
float velocidadSalto = 0; // Velocidad del salto
float direccionSalto = 0; // Dirección del salto actual
float velocidadHorizontal = 3.0f; // Velocidad de movimiento horizontal
int teclaApretada[256] = {0}; // Array para rastrear si una tecla está presionada
int teclaEspecialApretada[256] = {0}; // Array para rastrear teclas especiales (flechas)
const int N = 4; // cantidad de figuras
float posX[N] = {1.0f, 4.0f, 8.0f, 12.0f}; // posiciones iniciales ajustadas al área visible
float velX[N] = {3.0f, -4.0f, 5.0f, -2.5f}; // velocidades ajustadas
int ultimoMs = 0;
float mov = 0;


void arbolito(){
//triangulo 1
    glBegin(GL_TRIANGLES);
    glColor3ub(159, 74, 5);
    glVertex2f(0, 0); // *
    glVertex2f(0.804837852673, -0.466319975009);  // C
    glVertex2f(0, -1.124470509283);     // F
    glEnd();

    //triangulo 2
    glBegin(GL_TRIANGLES);
    glColor3ub(157, 72, 5);
    glVertex2f(0.0f, 0.0f);  // *
    glVertex2f(1.523339758092, 0);      // I₂
    glVertex2f(0.804837852673, -0.466319975009);  // C
    glEnd();


	//triangulo 3
    glBegin(GL_TRIANGLES);
    glColor3ub(190, 93, 7);
	glVertex2f(0, -1.124470509283);               // F
    glVertex2f(1.274029103238, -1.721382566863);  // D
    glVertex2f(0, -2.2);                // J
    glEnd();

    //triangulo 4
    glBegin(GL_TRIANGLES);
    glColor3ub(178, 85, 5);
    glVertex2f(0, -1.124470509283);               // F
    glVertex2f(0.804837852673, -0.466319975009);  // C
    glVertex2f(1.274029103238, -1.721382566863);  // D
    glEnd();

    //triangulo 5
    glBegin(GL_TRIANGLES);
    glColor3ub(196, 95, 4);
    glVertex2f(0.804837852673, -0.466319975009);  // C
    glVertex2f(2.655232540359, -1.167037067459);  // E
    glVertex2f(1.274029103238, -1.721382566863);  // D
    glEnd();

    //POLIGONO 6
    glBegin(GL_POLYGON);
    glColor3ub(178, 81, 4);
    glVertex2f(0.804837852673, -0.466319975009);  // C
    glVertex2f(1.523339758092, 0);                // I₂
    glVertex2f(1.825403295393, 0);                // Q
    glVertex2f(2.655232540359, -1.167037067459);  // E
    glEnd();

    //POLIGONO 7
    glBegin(GL_POLYGON);
    glColor3ub(192, 88, 2);   
    glVertex2f(1.825403295393, 0);                // Q
    glVertex2f(3.662491095984, 0);                // R
    glVertex2f(3.418955521454, -0.317909003667);  // O₁ 
    glVertex2f(2.655232540359, -1.167037067459);  // E
    glEnd();

    //POLIGONO 8
    glBegin(GL_POLYGON);
    glColor3ub(211, 101, 1);
    glVertex2f(3.418955521454, -0.317909003667);  // O₁ 
    glVertex2f(4.982895850166, -0.224694017187);  // P₁
    glVertex2f(4.255325454570, -0.934108191256);  // L₁
    glVertex2f(2.655232540359, -1.167037067459);  // E
    glEnd();

    //Poligono 9
    glBegin(GL_POLYGON);
    glColor3ub(200, 93, 2);
    glVertex2f(3.662491095984, 0);                // R
    glVertex2f(5.303969694285, 0);                // N₁
    glVertex2f(4.982895850166, -0.224694017187);  // P₁
    glVertex2f(3.418955521454, -0.317909003667);  // O₁ 
    glEnd();

    //Poligono 10
    glBegin(GL_POLYGON);
    glColor3ub(218, 107, 1);
    glVertex2f(4.982895850166, -0.224694017187);  // P₁
    glVertex2f(5.303969694285, 0);                // N₁
    glVertex2f(6.646909870614, -1.199141949557);  // Q₁
    glVertex2f(4.255325454570, -0.934108191256);  // L₁
    glEnd();

    //triangulo 11
    glBegin(GL_TRIANGLES);
    glColor3ub(188, 85, 3);
    glVertex2f(5.303969694285, 0);                // N₁
    glVertex2f(7.082080632375, 0);                // B₇
    glVertex2f(6.646909870614, -1.199141949557);  // Q₁
    glEnd();


    //triangulo 12
    glBegin(GL_TRIANGLES);
    glColor3ub(192, 88, 4);
    glVertex2f(7.082080632375, 0);                // B₇
    glVertex2f(8.574793771593, -0.465943590639);  // A₇
    glVertex2f(6.646909870614, -1.199141949557);  // Q₁
    glEnd();


    //triangulo 13
    glBegin(GL_TRIANGLES);
    glColor3ub(211, 101, 2);
    glVertex2f(6.646909870614, -1.199141949557);  // Q₁
    glVertex2f(8.574793771593, -0.465943590639);  // A₇
    glVertex2f(9, -1.5);                          // W₆
    glEnd();


    //poligonoo 14
    glBegin(GL_POLYGON);
    glColor3ub(227, 114, 1);
    glVertex2f(6.646909870614, -1.199141949557);  // Q₁
    glVertex2f(9, -1.5);                          // W₆
    glVertex2f(8.017864890716, -2.280655680981);  // V₆
    glVertex2f(6.926118869006, -1.999132665777);  // Z₆
    glEnd();


    //poligono 15 (triangulo)
    glBegin(GL_TRIANGLES);
    glColor3ub(231, 118, 0);
    glVertex2f(5.500422287530, -1.651128883546);  // M₁
    glVertex2f(6.646909870614, -1.199141949557);  // Q₁
    glVertex2f(6.926118869006, -1.999132665777);  // Z₆
    // glVertex2f(6.384940316954, -1.877559178776);  // R₁
    glEnd();


    //triangulo 16
    glBegin(GL_TRIANGLES);
    glColor3ub(228, 115, 2);
    glVertex2f(4.255325454570, -0.934108191256);  // L₁
    glVertex2f(6.646909870614, -1.199141949557);  // Q₁
    glVertex2f(5.500422287530, -1.651128883546);  // M₁
    glEnd();


    //triangulo 17
    glBegin(GL_POLYGON);
    glColor3ub(244, 135, 1);
    glVertex2f(4.255325454570, -0.934108191256);  // L₁
    glVertex2f(5.500422287530, -1.651128883546);  // M₁
    glVertex2f(4.349638400592, -1.783535565281);  // T₁
    glVertex2f(3.189425502518, -1.990154097131);  // S
    glEnd();


    //triangulo 18
    glBegin(GL_TRIANGLES);
    glColor3ub(232, 120, 0);
    glVertex2f(2.655232540359, -1.167037067459);  // E
    glVertex2f(4.255325454570, -0.934108191256);  // L₁
    glVertex2f(3.189425502518, -1.990154097131);  // S
    glEnd();

    //triangulo 19
    glBegin(GL_TRIANGLES);
    glColor3ub(230, 119, 1);
    glVertex2f(2.655232540359, -1.167037067459);  // E
    glVertex2f(3.189425502518, -1.990154097131);  // S
    glVertex2f(2.266865552354, -2.256123699170);  // I
    glEnd();

    //triangulo 20
    glBegin(GL_TRIANGLES);
    glColor3ub(219, 114, 0);
    glVertex2f(1.274029103238, -1.721382566863);  // D
    glVertex2f(2.655232540359, -1.167037067459);  // E
    glVertex2f(2.266865552354, -2.256123699170);  // I
    glEnd();

    //triangulo 21
    glBegin(GL_TRIANGLES);
    glColor3ub(218, 112, 2);
    glVertex2f(1.274029103238, -1.721382566863);  // D
    glVertex2f(2.266865552354, -2.256123699170);  // I
    glVertex2f(1.599581897596, -2.852672771225);  // H
    glEnd();

    //triangulo 22
    glBegin(GL_TRIANGLES);
    glColor3ub(211, 107, 4);
    glVertex2f(1.274029103238, -1.721382566863);  // D
    glVertex2f(1.599581897596, -2.852672771225);  // H
    glVertex2f(0.515700990739, -2.839835655486);  // G
    glEnd();

    //triangulo 23
    glBegin(GL_TRIANGLES);
    glColor3ub(197, 97, 5);
    glVertex2f(0, -2.2);                          // J
    glVertex2f(1.274029103238, -1.721382566863);  // D
    glVertex2f(0.515700990739, -2.839835655486);  // G
    glEnd();

    //triangulo 24
    glBegin(GL_POLYGON);
    glColor3ub(180, 70, 15);   // antes 234, 112, 0
    glVertex2f(0, -2.2);                          // J
    glVertex2f(1.067568177747, -4.208289297893);  // K
    glVertex2f(0, -3.690361895226);               // P
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(195, 90, 20);   // antes 251, 134, 0
    glVertex2f(0, -2.2);                          // J
    glVertex2f(0.515700990739, -2.839835655486);  // G
    glVertex2f(1.067568177747, -4.208289297893);  // K
    glEnd();

    //triangulo 25
    glBegin(GL_TRIANGLES);
    glColor3ub(218, 114, 6);
    glVertex2f(0.515700990739, -2.839835655486);  // G
    glVertex2f(1.359781159251, -3.445860694975);  // L
    glVertex2f(1.067568177747, -4.208289297893);  // K
    glEnd();

    //triangulo 26
    glBegin(GL_TRIANGLES);
    glColor3ub(221, 115, 3);
    glVertex2f(0.515700990739, -2.839835655486);  // G
    glVertex2f(1.599581897596, -2.852672771225);  // H
    glVertex2f(1.359781159251, -3.445860694975);  // L
    glEnd();

    //triangulo 27
    glBegin(GL_POLYGON);
    glColor3ub(238, 134, 1);
    glVertex2f(1.599581897596, -2.852672771225);  // H
    glVertex2f(2.379473429084, -2.980095520218);  // T
    glVertex2f(2.029123015472, -3.730828019901);  // M
    glVertex2f(1.359781159251, -3.445860694975);  // L
    glEnd();

    //triangulo 28
    glBegin(GL_POLYGON);
    glColor3ub(245, 137, 1);
    glVertex2f(2.266865552354, -2.256123699170);  // I
    glVertex2f(3.189425502518, -1.990154097131);  // S
    glVertex2f(2.379473429084, -2.980095520218);  // T
    glVertex2f(1.599581897596, -2.852672771225);  // H

    glEnd();

    //triangulo 29
    glBegin(GL_TRIANGLES);
    glColor3ub(228, 122, 1);
    glVertex2f(1.359781159251, -3.445860694975);  // L
    glVertex2f(2.029123015472, -3.730828019901);  // M
    glVertex2f(1.067568177747, -4.208289297893);  // K
    glEnd();

    //triangulo 30
    glBegin(GL_TRIANGLES);
    glColor3ub(252, 151, 0);
    glVertex2f(3.189425502518, -1.990154097131);  // S
    glVertex2f(3.556903758058, -3.190083094812);  // U
    glVertex2f(2.379473429084, -2.980095520218);  // T
    glEnd();


    //triangulo 31
    glBegin(GL_TRIANGLES);
    glColor3ub(252, 159, 3);
    glVertex2f(2.379473429084, -2.980095520218);  // T
    glVertex2f(3.556903758058, -3.190083094812);  // U
    glVertex2f(2.883443608110, -3.847044221043);  // K₁
    glEnd();

    //triangulo 32
    glBegin(GL_TRIANGLES);
    glColor3ub(249, 149, 2);
    glVertex2f(2.379473429084, -2.980095520218);  // T
    glVertex2f(2.883443608110, -3.847044221043);  // K₁
    glVertex2f(2.029123015472, -3.730828019901);  // M

    glEnd();

    //triangulo 33
    glBegin(GL_TRIANGLES);
    glColor3ub(245, 146, 1);
    glVertex2f(2.029123015472, -3.730828019901);  // M
    glVertex2f(2.883443608110, -3.847044221043);  // K₁
    glVertex2f(2.524022753481, -4.735180746920);  // W₁
    glEnd();

    //triangulo 34
    glBegin(GL_TRIANGLES);
    glColor3ub(250, 157, 1);
    glVertex2f(2.883443608110, -3.847044221043);  // K₁
    glVertex2f(3.565328468821, -4.358014137460);  // V₁
    glVertex2f(2.524022753481, -4.735180746920);  // W₁
    glEnd();

    //triangulo 35
    glBegin(GL_TRIANGLES);
    glColor3ub(251, 155, 2);
    glVertex2f(3.565328468821, -4.358014137460);  // V₁
    glVertex2f(3.796536357269, -5.212373348341);  // Z₁
    glVertex2f(2.524022753481, -4.735180746920);  // W₁
    glEnd();

    //triangulo 36
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 177, 7);
    glVertex2f(2.883443608110, -3.847044221043);  // K₁
    glVertex2f(3.616900207942, -3.775044841822);  // J₁
    glVertex2f(3.565328468821, -4.358014137460);  // V₁
    glEnd();

    //triangulo 37
    glBegin(GL_TRIANGLES);
    glColor3ub(254, 182, 10);
    glVertex2f(3.556903758058, -3.190083094812);  // U
    glVertex2f(3.616900207942, -3.775044841822);  // J₁    
    glVertex2f(2.883443608110, -3.847044221043);  // K₁
    glEnd();

    //triangulo 38
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 221, 53);
    glVertex2f(3.556903758058, -3.190083094812);  // U
    glVertex2f(4.130815941864, -3.105591262635);  // F₂
    glVertex2f(3.616900207942, -3.775044841822);  // J₁ 
    glEnd();

    //triangulo 39
    glBegin(GL_TRIANGLES);
    glColor3ub(252, 173, 7);
    glVertex2f(3.189425502518, -1.990154097131);  // S
    glVertex2f(3.819388226301, -2.717611051975);  // V
    glVertex2f(3.556903758058, -3.190083094812);  // U
    glEnd();

    //triangulo 40
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 162, 2);
    glVertex2f(3.189425502518, -1.990154097131);  // S
    glVertex2f(4.349638400592, -1.783535565281);  // T₁
    glVertex2f(3.819388226301, -2.717611051975);  // V
    glEnd();

    //triangulo 41
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 176, 9);
    glVertex2f(4.349638400592, -1.783535565281);  // T₁
    glVertex2f(4.659338524678, -2.335133683964);  // Z
    glVertex2f(3.819388226301, -2.717611051975);  // V
    glEnd();

    //triangulo 42
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 161, 2);
    glVertex2f(4.349638400592, -1.783535565281);  // T₁
    glVertex2f(5.500422287530, -1.651128883546);  // M₁
    glVertex2f(6.149839435465, -2.636615906686);  // S₁
    glEnd();

    //triangulo 43
    glBegin(GL_POLYGON);
    glColor3ub(253, 177, 7);
    glVertex2f(4.349638400592, -1.783535565281);  // T₁
    glVertex2f(6.149839435465, -2.636615906686);  // S₁
    glVertex2f(5.829629297417, -3.490065344232);  // D₁
    glVertex2f(4.659338524678, -2.335133683964);  // Z
    glEnd();

    //triangulo 44
    glBegin(GL_POLYGON);
    glColor3ub(243, 132, 1);
    glVertex2f(5.500422287530, -1.651128883546);  // M₁
    glVertex2f(6.926118869006, -1.999132665777);  // Z₆
    glVertex2f(7.372430135743, -2.967288399037);  // S₆
    glVertex2f(6.149839435465, -2.636615906686);  // S₁

    glEnd();

    //triangulo 45
    glBegin(GL_TRIANGLES);
    glColor3ub(236, 123, 0);
    glVertex2f(6.926118869006, -1.999132665777);  // Z₆
    glVertex2f(8.017864890716, -2.280655680981);  // V₆
    glVertex2f(7.372430135743, -2.967288399037);  // S₆
    glEnd();

    //triangulo 46
    glBegin(GL_TRIANGLES);
    glColor3ub(238, 128, 2);
    glVertex2f(8.017864890716, -2.280655680981);  // V₆
    glVertex2f(8.7294536150805, -2.8737932096502); //U6
    glVertex2f(7.372430135743, -2.967288399037);  // S₆
    glEnd();

    //triangulo 47
    glBegin(GL_TRIANGLES);
    glColor3ub(252, 147, 1);
    glVertex2f(6.149839435465, -2.636615906686);  // S₁
    glVertex2f(7.372430135743, -2.967288399037);  // S₆
    glVertex2f(6.486673929449, -3.750049697622);  // T₆
    glEnd();

    //triangulo 48
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 168, 4);
    glVertex2f(6.149839435465, -2.636615906686);  // S₁
    glVertex2f(6.486673929449, -3.750049697622);  // T₆
    glVertex2f(5.829629297417, -3.490065344232);  // D₁
    glEnd();

    //triangulo 49
    glBegin(GL_POLYGON);
    glColor3ub(253, 169, 3);
    glVertex2f(5.829629297417, -3.490065344232);  // D₁
    glVertex2f(6.486673929449, -3.750049697622);  // T₆
    glVertex2f(5.8783098807216, -4.2764658374726);  // E₂
    glVertex2f(5.694277285178, -4.000035168247);  // E₁
    glEnd();

    //triangulo 50
    glBegin(GL_POLYGON);
    glColor3ub(252, 180, 6);
    glVertex2f(3.969379351011, -4.300017417667);  // I₁
    glVertex2f(5.694277285178, -4.000035168247);  // E₁
    glVertex2f(5.8783098807216, -4.2764658374726);  // E₂
    glVertex2f(5.400976819800, -5.147555865114);  // A₃
    glVertex2f(4.343256600466, -4.7339811588028);   //G₁₃
    glEnd();

    //triangulo 51
    glBegin(GL_TRIANGLES);
    glColor3ub(252, 185, 8);
    glVertex2f(3.565328468821, -4.358014137460);  // V₁
    glVertex2f(3.969379351011, -4.300017417667);  // I₁
    glVertex2f(4.343256600466, -4.7339811588028); //G₁₃
    glEnd();

    //triangulo 52
    glBegin(GL_TRIANGLES);
    glColor3ub(251, 195, 16);
    glVertex2f(3.616900207942, -3.775044841822);  // J₁
    glVertex2f(3.969379351011, -4.300017417667);  // I₁
    glVertex2f(3.565328468821, -4.358014137460);  // V₁
    glEnd();

    //triangulo 53
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 167, 3);
    glVertex2f(5.8783098807216, -4.2764658374726);  // E₂
    glVertex2f(6.472941275088, -4.580875286471);  // R₆
    glVertex2f(5.400976819800, -5.147555865114);  // A₃
    glEnd();

    //triangulo 54
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 162, 2);
    glVertex2f(5.8783098807216, -4.2764658374726);  // E₂
    glVertex2f(6.486673929449, -3.750049697622);  // T₆
    glVertex2f(6.472941275088, -4.580875286471);  // R₆
    glEnd();

    //triangulo 55
    glBegin(GL_TRIANGLES);
    glColor3ub(252, 167, 2);
    glVertex2f(3.565328468821, -4.358014137460);  // V₁
    glVertex2f(4.343256600466, -4.7339811588028);   //G₁₃
    glVertex2f(3.796536357269, -5.212373348341);  // Z₁
    glEnd();

    //triangulo 56
    glBegin(GL_POLYGON);
    glColor3ub(251, 159, 0);
    glVertex2f(4.343256600466, -4.7339811588028);   //G₁₃
    glVertex2f(5.400976819800, -5.147555865114);  // A₃
    glVertex2f(4.603319953866, -5.523922333806);  // Z₂
	glVertex2f(3.796536357269, -5.212373348341);  // Z₁
    glEnd();

    //triangulo 57
    glBegin(GL_POLYGON);
    glColor3ub(246, 144, 2);
    glVertex2f(5.400976819800, -5.147555865114);  // A₃
    glVertex2f(5.970532226410, -5.305906989703);  // E₃
    glVertex2f(5.742378227052, -5.464009655942);  // G₃
    glVertex2f(5.134247333229, -5.849087920280);  // S₂
    glVertex2f(4.603319953866, -5.523922333806);  // Z₂
    glEnd();

    //triangulo 58
    glBegin(GL_TRIANGLES);
    glColor3ub(231, 127, 4);
    glVertex2f(3.796536357269, -5.212373348341);  // Z₁
    glVertex2f(4.413843244018, -5.742701627125);  // N₂
    glVertex2f(3.043037838573, -5.852366059563);  // M₂
    glEnd();

    //triangulo 59
    glBegin(GL_TRIANGLES);
    glColor3ub(236, 136, 2);
    glVertex2f(2.524022753481, -4.735180746920);  // W₁
    glVertex2f(3.796536357269, -5.212373348341);  // Z₁
    glVertex2f(3.043037838573, -5.852366059563);  // M₂
    glEnd();

    //triangulo 60
    glBegin(GL_TRIANGLES);
    glColor3ub(243, 140, 1);
    glVertex2f(3.796536357269, -5.212373348341);  // Z₁
    glVertex2f(4.603319953866, -5.523922333806);  // Z₂
    glVertex2f(4.413843244018, -5.742701627125);  // N₂
    glEnd();

    //triangulo 61
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 221, 52);
    glVertex2f(3.819388226301, -2.717611051975);  // V
    glVertex2f(4.171867369370, -2.455126583733);  // W
    glVertex2f(4.130815941864, -3.105591262635);  // F₂
    glEnd();

    //triangulo 62
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 227, 68);
    glVertex2f(4.171867369370, -2.455126583733);  // W
    glVertex2f(4.766370368964, -2.784161437435);  // G₂
    glVertex2f(4.130815941864, -3.105591262635);  // F₂
    glEnd();

    //triangulo 63
    glBegin(GL_TRIANGLES);
    glColor3ub(252, 220, 56);
    glVertex2f(4.171867369370, -2.455126583733);  // W
    glVertex2f(4.659338524678, -2.335133683964);  // Z
    glVertex2f(4.766370368964, -2.784161437435);  // G₂
    glEnd();

    //triangulo 64
    glBegin(GL_TRIANGLES);
    glColor3ub(252, 222, 55);
    glVertex2f(4.659338524678, -2.335133683964);  // Z
    glVertex2f(5.086813230102, -2.410129246319);  // A₁
    glVertex2f(4.766370368964, -2.784161437435);  // G₂
    glEnd();

    //triangulo 65
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 225, 69);
    glVertex2f(5.086813230102, -2.410129246319);  // A₁
    glVertex2f(5.350788232964, -3.090980816035);  // H₂
    glVertex2f(4.766370368964, -2.784161437435);  // G₂
    glEnd();

    //triangulo 66
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 220, 56);
    glVertex2f(5.086813230102, -2.410129246319);  // A₁
    glVertex2f(5.424293260699, -2.575119483501);  // B₁
    glVertex2f(5.350788232964, -3.090980816035);  // H₂
    glEnd();

    //triangulo 67
    glBegin(GL_TRIANGLES);
    glColor3ub(254, 220, 58);
    glVertex2f(5.424293260699, -2.575119483501);  // B₁
    glVertex2f(5.716775953884, -2.987595076453);  // C₁
    glVertex2f(5.350788232964, -3.090980816035);  // H₂
    glEnd();

    //triangulo 68
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 218, 49);
    glVertex2f(5.716775953884, -2.987595076453);  // C₁
    glVertex2f(5.829629297417, -3.490065344232);  // D₁
    glVertex2f(5.350788232964, -3.090980816035);  // H₂
    glEnd();

    //triangulo 69
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 222, 55);
    glVertex2f(5.350788232964, -3.090980816035);  // H₂
    glVertex2f(5.829629297417, -3.490065344232);  // D₁
    glVertex2f(5.563600325866, -3.796556746688);  // D₂
    glEnd();

    //triangulo 70
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 228, 75);
    glVertex2f(5.350788232964, -3.090980816035);  // H₂
    glVertex2f(5.563600325866, -3.796556746688);  // D₂
    glVertex2f(4.922163387623, -3.533600794525);  // C₂
    glEnd();

    //triangulo 71
    glBegin(GL_TRIANGLES);
    glColor3ub(254, 227, 70);
    glVertex2f(4.922163387623, -3.533600794525);  // C₂
    glVertex2f(5.563600325866, -3.796556746688);  // D₂
    glVertex2f(4.993052578093, -4.313962965823);  // A₂
    glEnd();

    //triangulo 72
    glBegin(GL_TRIANGLES);
    glColor3ub(254, 229, 78);
    glVertex2f(4.922163387623, -3.533600794525);  // C₂
    glVertex2f(4.993052578093, -4.313962965823);  // A₂
    glVertex2f(4.328499530140, -3.937185142430);  // B₂
    glEnd();

    //triangulo 73
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 232, 87);
    glVertex2f(4.130815941864, -3.105591262635);  // F₂
    glVertex2f(4.922163387623, -3.533600794525);  // C₂
    glVertex2f(4.328499530140, -3.937185142430);  // B₂
    glEnd();

    //triangulo 74
    glBegin(GL_TRIANGLES);
    glColor3ub(254, 231, 85);
    glVertex2f(4.766370368964, -2.784161437435);  // G₂
    glVertex2f(4.922163387623, -3.533600794525);  // C₂
    glVertex2f(4.130815941864, -3.105591262635);  // F₂
    glEnd();

    //triangulo 75
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 229, 77);
    glVertex2f(4.766370368964, -2.784161437435);  // G₂
    glVertex2f(5.350788232964, -3.090980816035);  // H₂
    glVertex2f(4.922163387623, -3.533600794525);  // C₂
    glEnd();

    //triangulo 76
    glBegin(GL_TRIANGLES);
    glColor3ub(252, 220, 53);
    glVertex2f(3.819388226301, -2.717611051975);  // V
    glVertex2f(4.130815941864, -3.105591262635);  // F₂
    glVertex2f(3.556903758058, -3.190083094812);  // U
    glEnd();

    //triangulo 77
    glBegin(GL_TRIANGLES);
    glColor3ub(254, 228, 72);
    glVertex2f(4.130815941864, -3.105591262635);  // F₂
    glVertex2f(4.328499530140, -3.937185142430);  // B₂
    glVertex2f(3.616900207942, -3.775044841822);  // J₁
    glEnd();

    //triangulo 78
    glBegin(GL_TRIANGLES);
    glColor3ub(254, 222, 52);
    glVertex2f(3.616900207942, -3.775044841822);  // J₁
    glVertex2f(4.328499530140, -3.937185142430);  // B₂
    glVertex2f(3.969379351011, -4.300017417667);  // I₁
    glEnd();

    //triangulo 79
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 221, 52);
    glVertex2f(4.328499530140, -3.937185142430);  // B₂
    glVertex2f(4.5023903459102, -4.5613480951152);  // H₁
    glVertex2f(3.969379351011, -4.300017417667);  // I₁
    glEnd();

    //triangulo 80
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 224, 62);
    glVertex2f(4.328499530140, -3.937185142430);  // B₂
    glVertex2f(4.993052578093, -4.313962965823);  // A₂
    glVertex2f(4.5023903459102, -4.5613480951152);  // H₁
    glEnd();

    //triangulo 81
    glBegin(GL_TRIANGLES);
    glColor3ub(252, 208, 34);
    glVertex2f(4.5023903459102, -4.5613480951152);  // H₁
    glVertex2f(4.993052578093, -4.313962965823);  // A₂
    glVertex2f(5.0273944481718, -4.5447053137755); // G₁
    glEnd();

    //triangulo 82
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 205, 29);
    glVertex2f(4.993052578093, -4.313962965823);  // A₂
    glVertex2f(5.379295923286, -4.322516086374);  // F₁
    glVertex2f(5.0273944481718, -4.5447053137755); // G₁
    glEnd();

    //triangulo 83 y 84
    glBegin(GL_POLYGON);
    glColor3ub(253, 215, 45);
    glVertex2f(4.993052578093, -4.313962965823);  // A₂
    glVertex2f(5.563600325866, -3.796556746688);  // D₂
    glVertex2f(5.694277285178, -4.000035168247);  // E₁
    glVertex2f(5.379295923286, -4.322516086374);  // F₁
    glEnd();

    //triangulo 85
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 216, 45);
    glVertex2f(5.563600325866, -3.796556746688);  // D₂
    glVertex2f(5.829629297417, -3.490065344232);  // D₁
    glVertex2f(5.694277285178, -4.000035168247);  // E₁
    glEnd();

    //triangulo 86
    glBegin(GL_TRIANGLES);
    glColor3ub(177, 81, 5);
    glVertex2f(7.082080632375, 0); //B7
    glVertex2f(8.574793771593, -0.465943590639);  // A₇
    glVertex2f(9.2185773732834, 0); // H13 
    glEnd();

    //triangulo 87
    glBegin(GL_POLYGON);
    glColor3ub(169, 79, 8);
    glVertex2f(9.2185773732834, 0); // H13 
    glVertex2f(11.585885645866, 0);  // N₆
    glVertex2f(11.323052276548, -0.679204547562);  // O₆
    glVertex2f(8.574793771593, -0.465943590639);  // A₇
    glEnd();

    //triangulo 88
    glBegin(GL_POLYGON);
    glColor3ub(197, 91, 5);
    glVertex2f(8.574793771593, -0.465943590639);  // A₇
    glVertex2f(11.323052276548, -0.679204547562);  // O₆
    glVertex2f(11.810211625630, -1.624183033973);  // G₆
    glVertex2f(10.421773363989, -1.013003420808);  // C₇
    glVertex2f(9, -1.5);                // W₆
    glEnd();

    //triangulo 89
    glBegin(GL_POLYGON);
    glColor3ub(177, 84, 8);
    glVertex2f(11.585885645866, 0);  // N₆
    glVertex2f(11.970967093936, -0.379696754619);  // M₆
    glVertex2f(11.810211625630, -1.624183033973);  // G₆
    glVertex2f(11.323052276548, -0.679204547562);  // O₆
    glEnd();

    //triangulo 90
    glBegin(GL_TRIANGLES);
    glColor3ub(150, 71, 10);
    glVertex2f(11.585885645866, 0);  // N₆
    glVertex2f(13.692593839087, 0);  // L₆
    glVertex2f(11.970967093936, -0.379696754619);  // M₆
    glEnd();

    //triangulo 91
    glBegin(GL_TRIANGLES);
    glColor3ub(153, 72, 11);
    glVertex2f(11.970967093936, -0.379696754619);  // M₆
    glVertex2f(13.692593839087, 0);  // L₆
    glVertex2f(13.449707870299, -1.099355953324);  // E₆
    glEnd();

    //triangulo 92
    glBegin(GL_TRIANGLES);
    glColor3ub(172, 82, 7);
    glVertex2f(11.970967093936, -0.379696754619);  // M₆
    glVertex2f(13.449707870299, -1.099355953324);  // E₆
    glVertex2f(11.810211625630, -1.624183033973);  // G₆
    glEnd();

    //triangulo 93
    glBegin(GL_POLYGON);
    glColor3ub(146, 71, 11);
    glVertex2f(13.692593839087, 0);  // L₆
    glVertex2f(14.736710432718, 0);     // K₆ 
    glVertex2f(14.743079625641, -0.227121769172);  // H₆
    glVertex2f(13.449707870299, -1.099355953324);  // E₆
    glEnd();

    //triangulo 94
    glBegin(GL_TRIANGLES);
    glColor3ub(144, 71, 13);
    glVertex2f(13.449707870299, -1.099355953324);  // E₆
    glVertex2f(14.743079625641, -0.227121769172);  // H₆
    glVertex2f(14.855753038373, -1.629086604801);  // A₆
    glEnd();

    //triangulo 95
    glBegin(GL_TRIANGLES);
    glColor3ub(143, 73, 14);
    glVertex2f(14.743079625641, -0.227121769172);  // H₆
    glVertex2f(15.817791761304, -1.094635160940);  // B₆
    glVertex2f(14.855753038373, -1.629086604801);  // A₆
    glEnd();

    //triangulo 96
    glBegin(GL_POLYGON);
    glColor3ub(120, 64, 16);
    glVertex2f(14.736710432718, 0);     // K₆ 
    glVertex2f(16.55, 0);  // J₆
    glVertex2f(15.817791761304, -1.094635160940);  // B₆
    glVertex2f(14.743079625641, -0.227121769172);  // H₆
    glEnd();

    
    //triangulo 97
    glBegin(GL_TRIANGLES);
    glColor3ub(132, 70, 15);
    glVertex2f(15.817791761304, -1.094635160940);  // B₆
    glVertex2f(16.55, 0);  // J₆
    glVertex2f(16.55, -1.721963208089);  // I₆
    glEnd();

    //triangulo 98
    glBegin(GL_TRIANGLES);
    glColor3ub(144, 73, 16);
    glVertex2f(15.817791761304, -1.094635160940);  // B₆
    glVertex2f(16.55, -1.721963208089);  // I₆
    glVertex2f(16.216858560745, -2.231335967681);  // W₅
    glEnd();

    //triangulo 99
    glBegin(GL_TRIANGLES);
    glColor3ub(153, 74, 14);
    glVertex2f(14.855753038373, -1.629086604801);  // A₆
    glVertex2f(15.817791761304, -1.094635160940);  // B₆
    glVertex2f(16.216858560745, -2.231335967681);  // W₅
    glEnd();

    //triangulo 100
    glBegin(GL_TRIANGLES);
    glColor3ub(164, 83, 13);
    glVertex2f(16.216858560745, -2.231335967681);  // W₅
    glVertex2f(16.55, -1.721963208089);  // I₆
    glVertex2f(16.55, -3.246789287462);  // T₅

    //triangulo 101
    glBegin(GL_TRIANGLES);
    glColor3ub(173, 86, 10);
    glVertex2f(14.855753038373, -1.629086604801);  // A₆
    glVertex2f(16.216858560745, -2.231335967681);  // W₅
    glVertex2f(15.363728691357, -3.043513909717);  // Z₅
    glEnd();

    //triangulo 102
    glBegin(GL_TRIANGLES);
    glColor3ub(172, 87, 10);
    glVertex2f(16.216858560745, -2.231335967681);  // W₅
    glVertex2f(16.55, -3.246789287462);  // T₅
    glVertex2f(15.363728691357, -3.043513909717);  // Z₅
    glEnd();

    //triangulo 103
    glBegin(GL_TRIANGLES);
    glColor3ub(182, 92, 10);
    glVertex2f(15.363728691357, -3.043513909717);  // Z₅
    glVertex2f(16.55, -3.246789287462);  // T₅
    glVertex2f(16.079688160868, -3.871441442724);  // S₅
    glEnd();

    //triangulo 104
    glBegin(GL_TRIANGLES);
    glColor3ub(176, 89, 10);
    glVertex2f(16.079688160868, -3.871441442724);  // S₅
    glVertex2f(16.55, -3.246789287462);  // T₅
    glVertex2f(16.522989690409, -4.269405315835);  // U₅
    glEnd();

    //triangulo 105
    glBegin(GL_TRIANGLES);
    // glColor3ub(189, 95, 8);
    glColor3ub(129, 60, 0);
    glVertex2f(16.079688160868, -3.871441442724);  // S₅
    glVertex2f(16.55, -4.269405315835);  // U₅
    glVertex2f(15.313985518934, -4.768119536568);  // V₅
    glEnd();

    //triangulo 106
    glBegin(GL_POLYGON);
    // glColor3ub(171, 88, 12);
    glColor3ub(111, 55, 0);  
    glVertex2f(15.313985518934, -4.768119536568);  // V₅
    glVertex2f(16.55, -4.269405315835);  // U₅
    glVertex2f(16.5344955911296, -5.2276133272334); //P13
    glVertex2f(16.55, -5.364559208130);  // Q₅
    glEnd();

    //triangulo 107
    glBegin(GL_POLYGON);
    // glColor3ub(199, 102, 7);
    glColor3ub(139, 65, 0);
    glVertex2f(15.363728691357, -3.043513909717);  // Z₅
    glVertex2f(16.079688160868, -3.871441442724);  // S₅
    glVertex2f(15.313985518934, -4.768119536568);  // V₅
    glVertex2f(14.525075386954, -4.307067197443);  // R₅
    glVertex2f(14.852040577912, -3.950404517709);  // F₁₃
    glEnd();

	glBegin(GL_TRIANGLES);
    glColor3ub(210, 108, 5);
    glVertex2f(13.691357230749, -3.606468067993);  // F₆
    glVertex2f(15.363728691357, -3.043513909717);  // Z₅
    glVertex2f(13.8431509098699, -4.0042596750649); //I13
	glEnd();   

    //triangulo 108
    glBegin(GL_POLYGON);
    // glColor3ub(210, 108, 5);
    glColor3ub(190, 94, 4);
    glVertex2f(13.8431509098699, -4.0042596750649); //I13
    glVertex2f(15.363728691357, -3.043513909717);  // Z₅
    glVertex2f(14.852040577912, -3.950404517709);  // F₁₃
    glVertex2f(14.525075386954, -4.307067197443);  // R₅
    glEnd();

    //triangulo *
    glBegin(GL_TRIANGLES);
    glColor3ub(205, 102, 4);
    glVertex2f(13, -2.5);               // D₆
    glVertex2f(14.077996207409, -2.845272582575);  // C₆
    glVertex2f(13.691357230749, -3.606468067993);  // F₆

    //triangulo 109
	glBegin(GL_TRIANGLES);
    glColor3ub(202, 102, 6);
    glVertex2f(14.077996207409, -2.845272582575);  // C₆
    glVertex2f(15.363728691357, -3.043513909717);  // Z₅
    glVertex2f(13.691357230749, -3.606468067993);  // F₆
    glEnd();

    //triangulo 110
    glBegin(GL_TRIANGLES);
    glColor3ub(185, 93, 9);
    glVertex2f(14.077996207409, -2.845272582575);  // C₆
    glVertex2f(14.855753038373, -1.629086604801);  // A₆
    glVertex2f(15.363728691357, -3.043513909717);  // Z₅
    glEnd();

    //triangulo 111
    glBegin(GL_TRIANGLES);
    glColor3ub(186, 94, 10);
    glVertex2f(13, -2.5);               // D₆
    glVertex2f(14.855753038373, -1.629086604801);  // A₆
    glVertex2f(14.077996207409, -2.845272582575);  // C₆
    glEnd();

    //triangulo 112
    glBegin(GL_TRIANGLES);
    glColor3ub(170, 84, 11);
    glVertex2f(13.449707870299, -1.099355953324);  // E₆
    glVertex2f(14.855753038373, -1.629086604801);  // A₆
    glVertex2f(13, -2.5);               // D₆
    glEnd();

    //triangulo 113
    glBegin(GL_TRIANGLES);
    glColor3ub(182, 89, 9);
    glVertex2f(11.810211625630, -1.624183033973);  // G₆
    glVertex2f(13.449707870299, -1.099355953324);  // E₆
    glVertex2f(13, -2.5);               // D₆
    glEnd();

    //triangulo 114
    glBegin(GL_TRIANGLES);
    glColor3ub(200, 98, 5);
    glVertex2f(11.304443875618, -2.599535865942);  // P₆
    glVertex2f(11.810211625630, -1.624183033973);  // G₆
    glVertex2f(13, -2.5);               // D₆
    glEnd();

    //triangulo 115
    glBegin(GL_POLYGON);
    glColor3ub(213, 107, 5);
    glVertex2f(11.304443875618, -2.599535865942);  // P₆
    glVertex2f(13, -2.5);               // D₆
    glVertex2f(12.579800451558, -2.870433036097);  // L₅
    glVertex2f(12.029824904335, -3.333316499382);  // D₇
    glEnd();

    //triangulo /
    glBegin(GL_POLYGON);
    glColor3ub(213, 108, 4);
    glVertex2f(12.579800451558, -2.870433036097);  // L₅
    glVertex2f(13, -2.5);               // D₆
    glVertex2f(13.691357230749, -3.606468067993);  // F₆
    glVertex2f(13.8431509098699, -4.0042596750649); //I13
    glVertex2f(13.207983823693, -3.363215226994);  // M₅
    glEnd();

    //triangulo 116
    glBegin(GL_POLYGON);
    glColor3ub(221, 115, 5);
    glVertex2f(11.054749784201, -3.287812219938);  // N₅
    glVertex2f(11.304443875618, -2.599535865942);  // P₆
    glVertex2f(12.029824904335, -3.333316499382);  // D₇
    glVertex2f(11.570003583529, -3.744947910083);  // K₅
    glEnd();

    //triangulo 117
    glBegin(GL_TRIANGLES);
    glColor3ub(220, 114, 2);
    glVertex2f(10.376517989903, -2.329035435626);  // D₄
    glVertex2f(11.304443875618, -2.599535865942);  // P₆
    glVertex2f(11.054749784201, -3.287812219938);  // N₅
    glEnd();

    //triangulo 118
    glBegin(GL_POLYGON);
    glColor3ub(208, 102, 4);
    glVertex2f(10.376517989903, -2.329035435626);  // D₄
    glVertex2f(10.888686820795, -1.904087701511);  // Q₆
    glVertex2f(11.810211625630, -1.624183033973);  // G₆
	glVertex2f(11.304443875618, -2.599535865942);  // P₆
    glEnd();

    //triangulo 119
    glBegin(GL_POLYGON);
    glColor3ub(202, 98, 5);
    glVertex2f(9, -1.5);                // W₆
    glVertex2f(10.421773363989, -1.013003420808);  // C₇
    glVertex2f(11.810211625630, -1.624183033973);  // G₆
    glVertex2f(10.888686820795, -1.904087701511);  // Q₆
    glEnd();

    //triangulo 120
    glBegin(GL_POLYGON);
    glColor3ub(214, 105, 5);
    glVertex2f(9, -1.5);                // W₆
    glVertex2f(10.888686820795, -1.904087701511);  // Q₆
    glVertex2f(10.376517989903, -2.329035435626);  // D₄
    glVertex2f(9.630967162694, -1.916497311237);  // C₄
    glEnd();

    //triangulo 121
    glBegin(GL_TRIANGLES);
    glColor3ub(221, 110, 1);
    glVertex2f(9, -1.5);                          // W₆
    glVertex2f(9.630967162694, -1.916497311237);  // C₄
    glVertex2f(8.7294536150805, -2.8737932096502); //U6
    glEnd();

    //triangulo 122
    glBegin(GL_TRIANGLES);
    glColor3ub(230, 118, 3);
    glVertex2f(8.017864890716, -2.280655680981);  // V₆
    glVertex2f(9, -1.5);                          // W₆
    glVertex2f(8.7294536150805, -2.8737932096502); //U6
    glEnd();

    //triangulo 123
    glBegin(GL_TRIANGLES);
    glColor3ub(239, 131, 0);
    glVertex2f(7.372430135743, -2.967288399037);  // S₆
    glVertex2f(8.7294536150805, -2.8737932096502); //U6
    glVertex2f(8.107283017505, -3.522653568756);  // Z₃
    glEnd();

    //triangulo 124
    glBegin(GL_POLYGON);
    glColor3ub(247, 144, 2);
    glVertex2f(7.372430135743, -2.967288399037);  // S₆
    glVertex2f(8.107283017505, -3.522653568756);  // Z₃
    glVertex2f(7.579385308634, -4.266526564510);  // V₃
    glVertex2f(6.908129234285, -4.442489531699);  // F₃
    glEnd();

    //triangulo 125
    glBegin(GL_TRIANGLES);
    glColor3ub(248, 143, 1);
    glVertex2f(6.486673929449, -3.750049697622);  // T₆
    glVertex2f(7.372430135743, -2.967288399037);  // S₆
    glVertex2f(6.908129234285, -4.442489531699);  // F₃
    glEnd();

    //triangulo 126
    glBegin(GL_TRIANGLES);
    glColor3ub(251, 155, 1);
    glVertex2f(6.486673929449, -3.750049697622);  // T₆
    glVertex2f(6.908129234285, -4.442489531699);  // F₃
    glVertex2f(6.472941275088, -4.580875286471);  // R₆
    glEnd();

    //triangulo 127
    glBegin(GL_POLYGON);
    glColor3ub(250, 156, 2);
    glVertex2f(5.400976819800, -5.147555865114);  // A₃
    glVertex2f(6.472941275088, -4.580875286471);  // R₆
    glVertex2f(6.908129234285, -4.442489531699);  // F₃
    glVertex2f(5.970532226410, -5.305906989703);  // E₃
    glEnd();

    //triangulo 128
    glBegin(GL_POLYGON);
    glColor3ub(239, 134, 1);
    glVertex2f(4.603319953866, -5.523922333806);  // Z₂
    glVertex2f(5.134247333229, -5.849087920280);  // S₂
    glVertex2f(4.953717156131, -5.950438195142);  // R₂
    glVertex2f(4.413843244018, -5.742701627125);  // N₂
    glEnd();

    //triangulo 129
    glBegin(GL_TRIANGLES);
    glColor3ub(213, 114, 4);
    glVertex2f(1.067568177747, -4.208289297893);  // K
    glVertex2f(2.524022753481, -4.735180746920);  // W₁
    glVertex2f(1.624317835205, -5.073818560791);  // L₃
    glEnd();

    //triangulo 130
    glBegin(GL_TRIANGLES);
    glColor3ub(185, 75, 15);   // antes 237, 117, 0
    glVertex2f(1.067568177747, -4.208289297893);  // K
    glVertex2f(1.624317835205, -5.073818560791);  // L₃
    glVertex2f(0.419976204322, -4.640255736734);  // N
    glEnd();

    //triangulo 131
    glBegin(GL_POLYGON);
    glColor3ub(170, 72, 18);   // antes 222, 110, 3
    glVertex2f(0, -3.690361895226);               // P    
    glVertex2f(1.067568177747, -4.208289297893);  // K
    glVertex2f(0.419976204322, -4.640255736734);  // N
    glVertex2f(0, -4.1);                          // O
    glEnd();

    //triangulo 132
    glBegin(GL_TRIANGLES);
    glColor3ub(175, 70, 17);   // antes 227, 106, 2
    glVertex2f(0, -4.1);                          // O
    glVertex2f(0.419976204322, -4.640255736734);  // N
    glVertex2f(0, -4.8);                          // J13
    glEnd();

    //triangulo 133
    glBegin(GL_POLYGON);
    glColor3ub(193, 103, 9);
    glVertex2f(0, -4.8);                          // J13
    glVertex2f(0.419976204322, -4.640255736734);  // N
    glVertex2f(1.624317835205, -5.073818560791);  // L₃
    glVertex2f(0.805365526205, -5.454828458556);  // K₃
	glVertex2f(0, -5.14);                          // K13
    glEnd();

    //triangulo 134
    glBegin(GL_TRIANGLES);
    glColor3ub(223, 120, 4);
    glVertex2f(1.624317835205, -5.073818560791);  // L₃
    glVertex2f(2.524022753481, -4.735180746920);  // W₁
    glVertex2f(3.043037838573, -5.852366059563);  // M₂
    glEnd();

    //triangulo 136
    glBegin(GL_TRIANGLES);
    glColor3ub(176, 94, 11);
    glVertex2f(0.805365526205, -5.454828458556);  // K₃
    glVertex2f(1.624317835205, -5.073818560791);  // L₃
    glVertex2f(1.429650352935, -5.722356140825);  // L₂
    glEnd();

    //triangulo 135
    glBegin(GL_POLYGON);
    glColor3ub(129, 80, 14);
    glVertex2f(0, -5.14);                          // K13
    glVertex2f(0.805365526205, -5.454828458556);  // K₃
    glVertex2f(1.429650352935, -5.722356140825);  // L₂
    glVertex2f(1.226818805494, -6.080294165720);  // K₂
    glVertex2f(0, -5.533307579177);     // Z₈
    glEnd();

    //triangulo 137
    glBegin(GL_TRIANGLES);
    glColor3ub(228, 126, 3);
    glVertex2f(1.067568177747, -4.208289297893);  // K
    glVertex2f(2.029123015472, -3.730828019901);  // M
    glVertex2f(2.524022753481, -4.735180746920);  // W₁
    glEnd();

    //triangulo 138
    glBegin(GL_TRIANGLES);
    glColor3ub(13, 38, 14);
    glVertex2f(0, -11.0348);            // A
    glVertex2f(1.833029309330, -10.536582152952);  // D₈
    glVertex2f(5.352600456739, -11.0348);  // F₈
    glEnd();

    //triangulo 139
    glBegin(GL_POLYGON);
    glColor3ub(13, 39, 15);
    glVertex2f(0, -9.494094872135);     // E₈
    glVertex2f(0.825610628335, -9.625034593979);  // L₇
    glVertex2f(2.150736397414, -9.884034118955);  // K₇
    glVertex2f(1.833029309330, -10.536582152952);  // D₈
    glVertex2f(0, -11.0348);            // A
    glEnd();

    //triangulo 140
    glBegin(GL_POLYGON);
    glColor3ub(21, 43, 12);
    glVertex2f(2.150736397414, -9.884034118955);  // K₇
    glVertex2f(5.081714649816, -10.449970630570);  // S₇
    glVertex2f(5.352600456739, -11.0348);  // F₈
    glVertex2f(1.833029309330, -10.536582152952);  // D₈
    glEnd();

    //triangulo 141
    glBegin(GL_TRIANGLES);
    glColor3ub(25, 47, 10);
    glVertex2f(0.825610628335, -9.625034593979);  // L₇
    glVertex2f(1.644938967026, -8.926351404510);  // J₇
    glVertex2f(2.150736397414, -9.884034118955);  // K₇
    glEnd();

    //triangulo 142
    glBegin(GL_TRIANGLES);
    glColor3ub(14, 40, 13);
    glVertex2f(0, -8);                  // E₇
    glVertex2f(0.968878841197, -8.716969810657);  // G₇
    glVertex2f(0, -9.494094872135);     // E₈
    glEnd();

    //triangulo BETA
    glBegin(GL_POLYGON);
    glColor3ub(14, 40, 13);
    glVertex2f(0.968878841197, -8.716969810657);  // G₇
    glVertex2f(1.644938967026, -8.926351404510);  // J₇
    glVertex2f(0.825610628335, -9.625034593979);  // L₇
    glVertex2f(0, -9.494094872135);     // E₈
    glEnd();

    //triangulo 143
    glBegin(GL_POLYGON);
    glColor3ub(14, 38, 15);
    glVertex2f(0, -8);                            // E₇
    glVertex2f(1.094702428566, -7.676505304971);  // I₇
    glVertex2f(1.849643952775, -8.300784098593);  // H₇
    glVertex2f(0.968878841197, -8.716969810657);  // G₇
    glEnd();

    //triangulo 144
    glBegin(GL_TRIANGLES);
    glColor3ub(29, 51, 18);
    glVertex2f(0, -7.361946562076);     // F₇   
    glVertex2f(0.904674583089, -7.112348804167);  // J₂
    glVertex2f(1.094702428566, -7.676505304971);  // I₇
    glEnd();

    //triangulo 145
    glBegin(GL_TRIANGLES);
    glColor3ub(24, 48, 16);
    glVertex2f(0, -7.361946562076);     // F₇
    glVertex2f(1.094702428566, -7.676505304971);  // I₇
    glVertex2f(0, -8);                  // E₇
    glEnd();

    //triangulo 146
    glBegin(GL_TRIANGLES);
    glColor3ub(25, 47, 20);
    glVertex2f(0, -6.6);
    glVertex2f(0.904674583089, -7.112348804167);  // J₂
    glVertex2f(0, -7.361946562076);     // F₇
    glEnd();

    //triangulo 147
    glBegin(GL_TRIANGLES);
    glColor3ub(37, 50, 17);
    glVertex2f(0, -5.730146345901);  // A₉
    glVertex2f(0.904674583089, -7.112348804167);  // J₂
    glVertex2f(0, -6.6);
    glEnd();

    //triangulo 148
    glBegin(GL_POLYGON);
    glColor3ub(66, 60, 14);
    glVertex2f(0, -5.533307579177);     // Z₈
    glVertex2f(1.226818805494, -6.080294165720);  // K₂
    glVertex2f(0.904674583089, -7.112348804167);  // J₂
    glVertex2f(0, -5.730146345901);  // A₉
    glEnd();

    //triangulo 149
    glBegin(GL_POLYGON);
    glColor3ub(23, 43, 12);
    glVertex2f(0.968878841197, -8.716969810657);  // G₇
    glVertex2f(1.849643952775, -8.300784098593);  // H₇
    glVertex2f(2.157362026259, -8.751051586387);  // N₇
    glVertex2f(1.644938967026, -8.926351404510);  // J₇
    glEnd();

    //triangulo 150
    glBegin(GL_TRIANGLES);
    glColor3ub(45, 59, 9);
    glVertex2f(1.644938967026, -8.926351404510);  // J₇
    glVertex2f(2.157362026259, -8.751051586387);  // N₇
    glVertex2f(2.965688745397, -9.426865728617);  // M₇
    glEnd();

    //triangulo 151
    glBegin(GL_TRIANGLES);
    glColor3ub(33, 52, 10);
    glVertex2f(1.644938967026, -8.926351404510);  // J₇
    glVertex2f(2.965688745397, -9.426865728617);  // M₇
    glVertex2f(2.150736397414, -9.884034118955);  // K₇
    glEnd();

    //triangulo 152
    glBegin(GL_POLYGON);
    glColor3ub(53, 65, 8);
    glVertex2f(2.157362026259, -8.751051586387);  // N₇
    glVertex2f(2.813299281953, -8.963071709440);  // P₇
    glVertex2f(4.227982944709, -9.502868568670);  // Q₇
    glVertex2f(2.965688745397, -9.426865728617);  // M₇
    glEnd();

    //triangulo 153
    glBegin(GL_TRIANGLES);
    glColor3ub(45, 59, 8);
    glVertex2f(2.965688745397, -9.426865728617);  // M₇
    glVertex2f(4.227982944709, -9.502868568670);  // Q₇
    glVertex2f(5.081714649816, -10.449970630570);  // S₇
    glEnd();

    //triangulo 154
    glBegin(GL_TRIANGLES);
    glColor3ub(32, 51, 11);
    glVertex2f(2.150736397414, -9.884034118955);  // K₇
    glVertex2f(2.965688745397, -9.426865728617);  // M₇
    glVertex2f(5.081714649816, -10.449970630570);  // S₇
    glEnd();

    //triangulo 155
    glBegin(GL_POLYGON);
    glColor3ub(51, 60, 7);
    glVertex2f(4.227982944709, -9.502868568670);  // Q₇
    glVertex2f(5.888614139146, -10.004551845711);  // W₈
    glVertex2f(6.204482272208, -10.091686263493);  // E₉
    glVertex2f(6.531323065378, -10.207434858967);  // B₈
    glVertex2f(5.081714649816, -10.449970630570);  // S₇
    glEnd();

    //triangulo 156
    glBegin(GL_POLYGON);
    glColor3ub(49, 55, 8);
    glVertex2f(5.081714649816, -10.449970630570);  // S₇
    glVertex2f(6.531323065378, -10.207434858967);  // B₈
    glVertex2f(7.115624271284, -10.477384410341);  // B₉
    glVertex2f(8.253351317003, -11.0348);  // C₈
    glEnd();

    //triangulo 157
    glBegin(GL_TRIANGLES);
    glColor3ub(25, 45, 11);
    glVertex2f(1.849643952775, -8.300784098593);  // H₇
    glVertex2f(3.290344558821, -8.353513855664);  // O₇
    glVertex2f(2.157362026259, -8.751051586387);  // N₇
    glEnd();

    //triangulo 158
    glBegin(GL_POLYGON);
    glColor3ub(41, 56, 8);
    glVertex2f(3.290344558821, -8.353513855664);  // O₇
    glVertex2f(4.492048197071, -8.796898200111);  // R₇
    glVertex2f(2.813299281953, -8.963071709440);  // P₇
    glVertex2f(2.157362026259, -8.751051586387);  // N₇
    glEnd();

    //triangulo 159
    glBegin(GL_POLYGON);
    glColor3ub(29, 52, 11);
    glVertex2f(2.813299281953, -8.963071709440);  // P₇
    glVertex2f(4.492048197071, -8.796898200111);  // R₇
    glVertex2f(5.712756565394, -9.658602520297);  // T₇
    glVertex2f(4.227982944709, -9.502868568670);  // Q₇
    glEnd();

    //triangulo 160
    glBegin(GL_TRIANGLES);
    glColor3ub(28, 48, 13);
    glVertex2f(4.227982944709, -9.502868568670);  // Q₇
    glVertex2f(5.712756565394, -9.658602520297);  // T₇
    glVertex2f(5.888614139146, -10.004551845711);  // W₈
    glEnd();

    //triangulo 161
    glBegin(GL_POLYGON);
    glColor3ub(43, 46, 13);
    glVertex2f(5.712756565394, -9.658602520297);  // T₇
    glVertex2f(6.645636413814, -9.446595267980);  // V₇
    glVertex2f(6.735054394006, -9.681186367017);  // Z₇
    glVertex2f(5.888614139146, -10.004551845711);  // W₈
    glEnd();

    //triangulo 162
    glBegin(GL_TRIANGLES);
    glColor3ub(38, 52, 11);
    glVertex2f(6.022876285544, -9.108415821772);  // U₇
    glVertex2f(6.645636413814, -9.446595267980);  // V₇
    glVertex2f(5.712756565394, -9.658602520297);  // T₇
    glEnd();

    //triangulo 163
    glBegin(GL_TRIANGLES);
    glColor3ub(41, 57, 10);
    glVertex2f(4.492048197071, -8.796898200111);  // R₇
    glVertex2f(6.022876285544, -9.108415821772);  // U₇
    glVertex2f(5.712756565394, -9.658602520297);  // T₇
    glEnd();

    //triangulo 164
    glBegin(GL_POLYGON);
    glColor3ub(55, 63, 9);
    glVertex2f(4.883307137436, -8.369263380555);  // G₈
    glVertex2f(6.303836314334, -8.919246249773);  // P₈
    glVertex2f(6.022876285544, -9.108415821772);  // U₇
    glVertex2f(4.492048197071, -8.796898200111);  // R₇
    glEnd();

    //triangulo 165
    glBegin(GL_POLYGON);
    glColor3ub(56, 63, 11);
    glVertex2f(6.303836314334, -8.919246249773);  // P₈
    glVertex2f(7.554307189036, -9.006080975504);  // W₇
    glVertex2f(6.645636413814, -9.446595267980);  // V₇
    glVertex2f(6.022876285544, -9.108415821772);  // U₇
    glEnd();

    //triangulo 166
    glBegin(GL_TRIANGLES);
    glColor3ub(58, 64, 8);
    glVertex2f(3.085643040255, -7.931190301189);  // I₈
    glVertex2f(4.883307137436, -8.369263380555);  // G₈
    glVertex2f(4.492048197071, -8.796898200111);  // R₇
    glEnd();

    //triangulo 167
    glBegin(GL_TRIANGLES);
    glColor3ub(55, 63, 8);
    glVertex2f(3.085643040255, -7.931190301189);  // I₈
    glVertex2f(4.492048197071, -8.796898200111);  // R₇
    glVertex2f(3.290344558821, -8.353513855664);  // O₇
    glEnd();

    //triangulo 168
    glBegin(GL_POLYGON);
    glColor3ub(59, 53, 9);
    glVertex2f(6.645636413814, -9.446595267980);  // V₇
    glVertex2f(7.554307189036, -9.006080975504);  // W₇
    glVertex2f(7.246229620460, -9.393601749747);  // A₈
    glVertex2f(6.735054394006, -9.681186367017);  // Z₇
    glEnd();

    //triangulo 169
    glBegin(GL_POLYGON);
    glColor3ub(72, 73, 6);
    glVertex2f(2.5, -7.5);              // L₈
    glVertex2f(3.678949185324, -7.737789211069);  // H₈
    glVertex2f(4.883307137436, -8.369263380555);  // G₈
    glVertex2f(3.085643040255, -7.931190301189);  // I₈
    glEnd();

    //triangulo 170
    glBegin(GL_TRIANGLES);
    glColor3ub(40, 53, 10);
    glVertex2f(2.5, -7.5);              // L₈
    glVertex2f(3.245687924472, -7.465281866292);  // K₈
    glVertex2f(3.678949185324, -7.737789211069);  // H₈
    glEnd();

    //triangulo 171
    glBegin(GL_POLYGON);
    glColor3ub(58, 66, 9);
    glVertex2f(3.245687924472, -7.465281866292);  // K₈
    glVertex2f(4.088590968813, -7.632439854384);  // J₈
    glVertex2f(4.799957711238, -7.955670893714);  // O₈
    glVertex2f(3.678949185324, -7.737789211069);  // H₈
    glEnd();

    //triangulo 172
    glBegin(GL_POLYGON);
    glColor3ub(85, 80, 5);
    glVertex2f(3.678949185324, -7.737789211069);  // H₈
    glVertex2f(4.799957711238, -7.955670893714);  // O₈
    glVertex2f(5.556734412221, -8.232185457534);  // M₈
    glVertex2f(4.883307137436, -8.369263380555);  // G₈
    glEnd();

    //triangulo 173
    glBegin(GL_POLYGON);
    glColor3ub(81, 79, 5);
    glVertex2f(4.883307137436, -8.369263380555);  // G₈
    glVertex2f(5.556734412221, -8.232185457534);  // M₈
    glVertex2f(7.554307189036, -9.006080975504);  // W₇
    glVertex2f(6.303836314334, -8.919246249773);  // P₈
    glEnd();

    //triangulo 174
    glBegin(GL_POLYGON);
    glColor3ub(67, 70, 8);
    glVertex2f(5.556734412221, -8.232185457534);  // M₈
    glVertex2f(6.885937050092, -8.508743238373);  // Q₈
    glVertex2f(6.989216683413, -8.587023243333);  // P₉
    glVertex2f(7.554307189036, -9.006080975504);  // W₇
    glEnd();

    //triangulo 175
    glBegin(GL_POLYGON);
    glColor3ub(96, 88, 5);
    glVertex2f(4.833915640129, -7.620942737510);  // N₈
    glVertex2f(6.016215059779, -8.098012271896);  // R₈
    glVertex2f(6.885937050092, -8.508743238373);  // Q₈
    glVertex2f(5.556734412221, -8.232185457534);  // M₈
    glEnd();

    //triangulo 176
    glBegin(GL_TRIANGLES);
    glColor3ub(92, 84, 5);
    glVertex2f(4.833915640129, -7.620942737510);  // N₈
    glVertex2f(5.556734412221, -8.232185457534);  // M₈
    glVertex2f(4.799957711238, -7.955670893714);  // O₈
    glEnd();

    //triangulo 177
    glBegin(GL_TRIANGLES);
    glColor3ub(70, 71, 9);
    glVertex2f(6.016215059779, -8.098012271896);  // R₈
    glVertex2f(7.006530181202, -8.159873203602);  // T₈
    glVertex2f(6.885937050092, -8.508743238373);  // Q₈
    glEnd();

    //triangulo 178
    glBegin(GL_POLYGON);
    glColor3ub(66, 63, 8);
    glVertex2f(7.006530181202, -8.159873203602);  // T₈
    glVertex2f(7.900579023862, -8.112461525522);  // S₈
    glVertex2f(7.906088258667, -8.309876343555);  // L₁₁
    glVertex2f(6.989216683413, -8.587023243333);  // P₉
    glVertex2f(6.885937050092, -8.508743238373);  // Q₈
    glEnd();

    //triangulo 179
    glBegin(GL_TRIANGLES);
    glColor3ub(68, 67, 8);
    glVertex2f(6.885937050092, -8.508743238373);  // Q₈
    glVertex2f(7.906088258667, -8.309876343555);  // L₁₁
    glVertex2f(6.989216683413, -8.587023243333);  // P₉
    glEnd();

    //triangulo 180
    glBegin(GL_POLYGON);
    glColor3ub(78, 74, 8);
    glVertex2f(4.833915640129, -7.620942737510);  // N₈
    glVertex2f(6.146346825006, -7.699302587686);  // U₈
    glVertex2f(7.006530181202, -8.159873203602);  // T₈
    glVertex2f(6.016215059779, -8.098012271896);  // R₈
    glEnd();

    //triangulo 181
    glBegin(GL_TRIANGLES);
    glColor3ub(88, 75, 6);
    glVertex2f(6.146346825006, -7.699302587686);  // U₈
    glVertex2f(7.900579023862, -8.112461525522);  // S₈
    glVertex2f(7.006530181202, -8.159873203602);  // T₈
    glEnd();

    //triangulo 182
    glBegin(GL_POLYGON);
    glColor3ub(79, 70, 7);
    glVertex2f(5.8754352662519, -7.3482890292126); // L13
    glVertex2f(7.189403808110, -7.638344712050);  // V₈
    glVertex2f(7.900579023862, -8.112461525522);  // S₈
    glVertex2f(6.146346825006, -7.699302587686);  // U₈
    glEnd();

    //triangulo 183
    glBegin(GL_POLYGON);
    glColor3ub(84, 72, 8);
    glVertex2f(7.189403808110, -7.638344712050);  // V₈
    glVertex2f(9.624515555343, -7.783925047148);  // Z₁₁
    glVertex2f(8.944806945511, -7.991564491927);  // W₁₁
    glVertex2f(7.900579023862, -8.112461525522);  // S₈
    glEnd();

    //triangulo 184
    glBegin(GL_TRIANGLES);
    glColor3ub(84, 67, 6);
    glVertex2f(7.900579023862, -8.112461525522);  // S₈
    glVertex2f(8.944806945511, -7.991564491927);  // W₁₁
    glVertex2f(7.906088258667, -8.309876343555);  // L₁₁
    glEnd();

    //triangulo 185
    glBegin(GL_TRIANGLES);
    glColor3ub(97, 77, 5);
    glVertex2f(7.189403808110, -7.638344712050);  // V₈
    glVertex2f(7.960546210965, -7.424111245384); //M13
    glVertex2f(9.624515555343, -7.783925047148);  // Z₁₁
    glEnd();

    //triangulo 186
    glBegin(GL_POLYGON);
    glColor3ub(98, 80, 8);
    glVertex2f(5.6509635208512, -7.0043248240814); //N13
    glVertex2f(7.960546210965, -7.424111245384); //M13
    glVertex2f(7.189403808110, -7.638344712050);  // V₈
    glVertex2f(5.8754352662519, -7.3482890292126); // L13
    glEnd();

    //triangulo x
    glBegin(GL_TRIANGLES);
    glColor3ub(121, 90, 4);
    glVertex2f(7.960546210965, -7.424111245384); //M13
    glVertex2f(8.153066947219, -7.371261009027);  // A₁₂
    glVertex2f(9.624515555343, -7.783925047148);  // Z₁₁
    glEnd();

    //triangulo 187
    glBegin(GL_POLYGON);
    glColor3ub(137, 97, 3);
    glVertex2f(6.8, -7);                // B₁₂
    glVertex2f(7.425783926996, -7.192293722142);  // C₁₂
    glVertex2f(8.153066947219, -7.371261009027);  // A₁₂
    glVertex2f(7.960546210965, -7.424111245384); //M13
    glVertex2f(5.6509635208512, -7.0043248240814); //N13
    glEnd();

    //triangulo 188
    glBegin(GL_POLYGON);
    glColor3ub(180, 98, 8);
    glVertex2f(1.624317835205, -5.073818560791);  // L₃
    glVertex2f(3.043037838573, -5.852366059563);  // M₂
    glVertex2f(2.5, -7.5);              // L₈
    glVertex2f(1.226818805494, -6.080294165720);  // K₂
    glEnd();

    //triangulo 189
    glBegin(GL_TRIANGLES);
    glColor3ub(172, 95, 10);
    glVertex2f(3.043037838573, -5.852366059563);  // M₂
    glVertex2f(3.985155008149, -6.171389630317);  // O₂
    glVertex2f(3.921951328103, -6.834406754657);  // R₃
    glEnd();

    //triangulo 190
    glBegin(GL_TRIANGLES);
    glColor3ub(216, 113, 5);
    glVertex2f(3.043037838573, -5.852366059563);  // M₂
    glVertex2f(4.413843244018, -5.742701627125);  // N₂
    glVertex2f(3.985155008149, -6.171389630317);  // O₂
    glEnd();

    //triangulo ALFA
    glBegin(GL_TRIANGLES);
    glColor3ub(32, 47, 12);
    glVertex2f(5.081714649816, -10.449970630570);  // S₇
    glVertex2f(8.253351317003, -11.014211400947);  // C₈
    glVertex2f(5.352600456739, -11.0348);  // F₈
    glEnd();

    //triangulo 191
    glBegin(GL_POLYGON);
    glColor3ub(209, 109, 7);
    glVertex2f(3.985155008149, -6.171389630317);  // O₂
    glVertex2f(4.413843244018, -5.742701627125);  // N₂
    glVertex2f(4.767760275999, -6.525306894975);  // P₂
    glVertex2f(4.339072040107, -6.395703474821);  // Q₂
    glEnd();

    //triangulo 192
    glBegin(GL_POLYGON);
    glColor3ub(216, 111, 4);
    glVertex2f(4.413843244018, -5.742701627125);  // N₂
    glVertex2f(4.953717156131, -5.950438195142);  // R₂
    glVertex2f(5.441811185061, -6.131060266523);  // T₂
    glVertex2f(4.767760275999, -6.525306894975);  // P₂
    glEnd();

    //triangulo 193
    glBegin(GL_TRIANGLES);
    glColor3ub(194, 103, 7);
    glVertex2f(4.767760275999, -6.525306894975);  // P₂
    glVertex2f(5.441811185061, -6.131060266523);  // T₂
    glVertex2f(5.087015074736, -6.609101341486);  // V₂
    glEnd();

    //triangulo 194
    glBegin(GL_TRIANGLES);
    glColor3ub(199, 100, 6);
    glVertex2f(5.087015074736, -6.609101341486);  // V₂
    glVertex2f(5.441811185061, -6.131060266523);  // T₂
    glVertex2f(6.151403405709, -6.239366447569);  // U₂
    glEnd();

	//triangulo Y
	glBegin(GL_POLYGON);
	glColor3ub(135, 89, 7);
    glVertex2f(5.087015074736, -6.609101341486);  // V₂
    glVertex2f(6.432264481184, -6.684504342836);  // W₂
    glVertex2f(7.463209191119, -6.967986148459);  // N₃
    glVertex2f(6.8, -7);                // B₁₂
    glVertex2f(5.6509635208512, -7.0043248240814); //N13
    glEnd();


    //triangulo 195
    glBegin(GL_TRIANGLES);
    glColor3ub(181, 94, 11);
    glVertex2f(5.087015074736, -6.609101341486);  // V₂
    glVertex2f(6.151403405709, -6.239366447569);  // U₂
    glVertex2f(6.432264481184, -6.684504342836);  // W₂
    glEnd();

    //triangulo 196
    glBegin(GL_POLYGON);
    glColor3ub(237, 117, 0);
    glVertex2f(4.953717156131, -5.950438195142);  // R₂
    glVertex2f(5.134247333229, -5.849087920280);  // S₂
    glVertex2f(5.742378227052, -5.464009655942);  // G₃
    glVertex2f(5.970532226410, -5.305906989703);  // E₃
    glVertex2f(6.115403334984, -5.368561164444);  // B₃
    glVertex2f(5.441811185061, -6.131060266523);  // T₂
    glEnd();

    //triangulo 197
    glBegin(GL_TRIANGLES);
    glColor3ub(222, 110, 3);
    glVertex2f(5.441811185061, -6.131060266523);  // T₂
    glVertex2f(6.115403334984, -5.368561164444);  // B₃
    glVertex2f(6.151403405709, -6.239366447569);  // U₂
    glEnd();

    //triangulo 198
    glBegin(GL_TRIANGLES);
    glColor3ub(227, 106, 2);
    glVertex2f(6.115403334984, -5.368561164444);  // B₃
    glVertex2f(7.020772404915, -5.027627205056);  // C₃
    glVertex2f(6.151403405709, -6.239366447569);  // U₂
    glEnd();

    //triangulo 199
    glBegin(GL_TRIANGLES);
    glColor3ub(251, 134, 0);
    glVertex2f(5.970532226410, -5.305906989703);  // E₃
    glVertex2f(6.908129234285, -4.442489531699);  // F₃
    glVertex2f(6.115403334984, -5.368561164444);  // B₃
    glEnd();

    //triangulo 200
    glBegin(GL_TRIANGLES);
    glColor3ub(234, 112, 0);
    glVertex2f(6.115403334984, -5.368561164444);  // B₃
    glVertex2f(6.908129234285, -4.442489531699);  // F₃
    glVertex2f(7.020772404915, -5.027627205056);  // C₃
    glEnd();

    //triangulo 201
    glBegin(GL_TRIANGLES);
    glColor3ub(232, 108, 0);
    glVertex2f(6.908129234285, -4.442489531699);  // F₃
    glVertex2f(7.579385308634, -4.266526564510);  // V₃
    glVertex2f(7.020772404915, -5.027627205056);  // C₃
    glEnd();

    //triangulo 202
    glBegin(GL_POLYGON);
    glColor3ub(220, 102, 3);
    glVertex2f(7.020772404915, -5.027627205056);  // C₃
    glVertex2f(7.579385308634, -4.266526564510);  // V₃
    glVertex2f(7.570054895040, -5.038991670369);  // D₃
    glVertex2f(6.151403405709, -6.239366447569);  // U₂
    glEnd();

    //triangulo 203
    glBegin(GL_TRIANGLES);
    glColor3ub(204, 94, 5);
    glVertex2f(6.151403405709, -6.239366447569);  // U₂
    glVertex2f(7.570054895040, -5.038991670369);  // D₃
    glVertex2f(7.159991413291, -6.049415947493);  // H₃
    glEnd();

    //triangulo 204
    glBegin(GL_TRIANGLES);
    glColor3ub(179, 85, 8);
    glVertex2f(6.151403405709, -6.239366447569);  // U₂
    glVertex2f(7.159991413291, -6.049415947493);  // H₃
    glVertex2f(6.432264481184, -6.684504342836);  // W₂
    glEnd();

    //triangulo 205
    glBegin(GL_TRIANGLES);
    glColor3ub(163, 81, 13);
    glVertex2f(6.432264481184, -6.684504342836);  // W₂
    glVertex2f(7.159991413291, -6.049415947493);  // H₃
    glVertex2f(7.560607773458, -6.369909035627);  // I₃
    glEnd();

    //triangulo 206
    glBegin(GL_TRIANGLES);
    glColor3ub(194, 87, 6);
    glVertex2f(7.159991413291, -6.049415947493);  // H₃
    glVertex2f(7.932083983413, -5.376866058000);  // J₃
    glVertex2f(7.560607773458, -6.369909035627);  // I₃
    glEnd();

    //triangulo 207
    glBegin(GL_POLYGON);
    glColor3ub(217, 98, 1);
    glVertex2f(7.570054895040, -5.038991670369);  // D₃
    glVertex2f(8.220062631888, -4.488710278669);  // P₃
    glVertex2f(7.932083983413, -5.376866058000);  // J₃
	glVertex2f(7.159991413291, -6.049415947493);  // H₃
    glEnd();

    //triangulo 208
    glBegin(GL_TRIANGLES);
    glColor3ub(185, 83, 8);
    glVertex2f(7.932083983413, -5.376866058000);  // J₃
    glVertex2f(8.626677425418, -5.710091515387);  // M₃
    glVertex2f(7.560607773458, -6.369909035627);  // I₃
    glEnd();

    //triangulo 209
    glBegin(GL_TRIANGLES);
    glColor3ub(223, 100, 1);
    glVertex2f(7.932083983413, -5.376866058000);  // J₃
    glVertex2f(8.755445868264, -4.595786929944);  // Q₃
    glVertex2f(8.626677425418, -5.710091515387);  // M₃
    glEnd();

    //triangulo 210
    glBegin(GL_TRIANGLES);
    glColor3ub(220, 101, 1);
    glVertex2f(8.220062631888, -4.488710278669);  // P₃
    glVertex2f(8.755445868264, -4.595786929944);  // Q₃
    glVertex2f(7.932083983413, -5.376866058000);  // J₃
    glEnd();

    //triangulo 211
    glBegin(GL_TRIANGLES);
    glColor3ub(234, 109, 0);
    glVertex2f(7.579385308634, -4.266526564510);  // V₃
    glVertex2f(8.220062631888, -4.488710278669);  // P₃
    glVertex2f(7.570054895040, -5.038991670369);  // D₃
    glEnd();

    //triangulo 212
    glBegin(GL_TRIANGLES);
    glColor3ub(244, 123, 1);
    glVertex2f(7.579385308634, -4.266526564510);  // V₃
    glVertex2f(8.107283017505, -3.522653568756);  // Z₃
    glVertex2f(8.543304511694, -3.403195625143);  // W₃
    glEnd();

    //triangulo 213
    glBegin(GL_TRIANGLES);
    glColor3ub(233, 111, 1);
    glVertex2f(7.579385308634, -4.266526564510);  // V₃
    glVertex2f(8.543304511694, -3.403195625143);  // W₃
    glVertex2f(8.220062631888, -4.488710278669);  // P₃
    glEnd();

    //triangulo 214
    glBegin(GL_TRIANGLES);
    glColor3ub(238, 119, 0);
    glVertex2f(8.543304511694, -3.403195625143);  // W₃
    glVertex2f(9.081137337059, -3.721327639863);  // U₃
    glVertex2f(8.220062631888, -4.488710278669);  // P₃
    glEnd();

    //triangulo 215
    glBegin(GL_TRIANGLES);
    glColor3ub(222, 100, 1);
    glVertex2f(8.220062631888, -4.488710278669);  // P₃
    glVertex2f(9.081137337059, -3.721327639863);  // U₃
    glVertex2f(8.755445868264, -4.595786929944);  // Q₃
    glEnd();

    //triangulo 216
    glBegin(GL_TRIANGLES);
    glColor3ub(196, 80, 2);
    glVertex2f(9.081137337059, -3.721327639863);  // U₃
    glVertex2f(9.607597519496, -3.948865551523);  // T₃
    glVertex2f(8.755445868264, -4.595786929944);  // Q₃
    glEnd();

    //triangulo 217
    glBegin(GL_TRIANGLES);
    glColor3ub(214, 92, 1);
    glVertex2f(8.755445868264, -4.595786929944);  // Q₃
    glVertex2f(9.607597519496, -3.948865551523);  // T₃
    glVertex2f(9.112368025848, -5.024093515045);  // S₃
    glEnd();

    //triangulo 218
    glBegin(GL_TRIANGLES);
    glColor3ub(216, 95, 1);
    glVertex2f(8.755445868264, -4.595786929944);  // Q₃
    glVertex2f(9.112368025848, -5.024093515045);  // S₃
    glVertex2f(8.626677425418, -5.710091515387);  // M₃
    glEnd();

    //triangulo 219
    glBegin(GL_TRIANGLES);
    glColor3ub(179, 76, 7);
    glVertex2f(9.112368025848, -5.024093515045);  // S₃
    glVertex2f(9.577662133400, -5.395829415049);  // H₄
    glVertex2f(8.626677425418, -5.710091515387);  // M₃
    glEnd();

    //triangulo 220
    glBegin(GL_TRIANGLES);
    glColor3ub(200, 82, 3);
    glVertex2f(9.112368025848, -5.024093515045);  // S₃
    glVertex2f(10.108119692108, -4.566687917255);  // G₄
    glVertex2f(9.577662133400, -5.395829415049);  // H₄
    glEnd();

    //triangulo 221
    glBegin(GL_TRIANGLES);
    glColor3ub(201, 83, 2);
    glVertex2f(9.607597519496, -3.948865551523);  // T₃
    glVertex2f(10.108119692108, -4.566687917255);  // G₄
    glVertex2f(9.112368025848, -5.024093515045);  // S₃
    glEnd();

    //triangulo 222
    glBegin(GL_TRIANGLES);
    glColor3ub(177, 70, 7);
    glVertex2f(9.607597519496, -3.948865551523);  // T₃
    glVertex2f(10.431191717232, -3.914573528159);  // F₄
    glVertex2f(10.108119692108, -4.566687917255);  // G₄
    glEnd();

    //triangulo 223
    glBegin(GL_TRIANGLES);
    glColor3ub(150, 60, 12);
    glVertex2f(9.993010472054, -2.884266775642);  // B₄
    glVertex2f(10.431191717232, -3.914573528159);  // F₄
    glVertex2f(9.607597519496, -3.948865551523);  // T₃
    glEnd();

    //triangulo 224
    glBegin(GL_TRIANGLES);
    glColor3ub(201, 81, 3);
    glVertex2f(9.993010472054, -2.884266775642);  // B₄
    glVertex2f(9.607597519496, -3.948865551523);  // T₃
    glVertex2f(9.081137337059, -3.721327639863);  // U₃
    glEnd();

    //triangulo 225
    glBegin(GL_TRIANGLES);
    glColor3ub(205, 82, 4);
    glVertex2f(9.389283971632, -2.840447916741);  // A₄
    glVertex2f(9.993010472054, -2.884266775642);  // B₄ 
    glVertex2f(9.081137337059, -3.721327639863);  // U₃
    glEnd();

    //triangulo 226
    glBegin(GL_TRIANGLES);
    glColor3ub(246, 119, 0);
    glVertex2f(8.543304511694, -3.403195625143);  // W₃
    glVertex2f(9.389283971632, -2.840447916741);  // A₄
    glVertex2f(9.081137337059, -3.721327639863);  // U₃
    glEnd();

    //triangulo 227
    glBegin(GL_POLYGON);
    glColor3ub(252, 135, 1);
    glVertex2f(8.7294536150805, -2.8737932096502); //U6
    glVertex2f(9.630967162694, -1.916497311237);  // C₄
    glVertex2f(9.389283971632, -2.840447916741);  // A₄
    glVertex2f(8.543304511694, -3.403195625143);  // W₃
    glEnd();

    //triangulo 228
    glBegin(GL_TRIANGLES);
    glColor3ub(253, 146, 1);
    glVertex2f(8.107283017505, -3.522653568756);  // Z₃
    glVertex2f(8.7294536150805, -2.8737932096502); //U6
    glVertex2f(8.543304511694, -3.403195625143);  // W₃
    glEnd();

    //triangulo 229
    glBegin(GL_TRIANGLES);
    glColor3ub(187, 79, 7);
    glVertex2f(9.630967162694, -1.916497311237);  // C₄
    glVertex2f(9.993010472054, -2.884266775642);  // B₄
    glVertex2f(9.389283971632, -2.840447916741);  // A₄
    glEnd();

    //triangulo 230
    glBegin(GL_TRIANGLES);
    glColor3ub(144, 60, 10);
    glVertex2f(9.630967162694, -1.916497311237);  // C₄
    glVertex2f(10.376517989903, -2.329035435626);  // D₄
    glVertex2f(9.993010472054, -2.884266775642);  // B₄
    glEnd();

    //triangulo 231
    glBegin(GL_TRIANGLES);
    glColor3ub(121, 51, 14);
    glVertex2f(9.993010472054, -2.884266775642);  // B₄
    glVertex2f(10.376517989903, -2.329035435626);  // D₄
    glVertex2f(10.853670519318, -3.492094726073);  // E₄
    glEnd();

    //triangulo 232
    glBegin(GL_TRIANGLES);
    glColor3ub(119, 49, 14);
    glVertex2f(9.993010472054, -2.884266775642);  // B₄
    glVertex2f(10.853670519318, -3.492094726073);  // E₄
    glVertex2f(10.431191717232, -3.914573528159);  // F₄
    glEnd();

    //triangulo 233
    glBegin(GL_TRIANGLES);
    glColor3ub(117, 51, 12);
    glVertex2f(10.376517989903, -2.329035435626);  // D₄
    glVertex2f(11.054749784201, -3.287812219938);  // N₅
    glVertex2f(10.853670519318, -3.492094726073);  // E₄
    glEnd();

    //triangulo 234
    glBegin(GL_TRIANGLES);
    glColor3ub(112, 46, 15);
    glVertex2f(10.431191717232, -3.914573528159);  // F₄
    glVertex2f(10.853670519318, -3.492094726073);  // E₄
    glVertex2f(11.424826833770, -4.567194783107);  // L₄
    glEnd();

    //triangulo 235
    glBegin(GL_TRIANGLES);
    glColor3ub(114, 49, 16);
    glVertex2f(10.853670519318, -3.492094726073);  // E₄
    glVertex2f(12.091008932919, -4.219336937975);  // V₄
    glVertex2f(11.424826833770, -4.567194783107);  // L₄
    glEnd();

    //triangulo 236
    glBegin(GL_POLYGON);
    glColor3ub(118, 51, 14);
    glVertex2f(10.853670519318, -3.492094726073);  // E₄
    glVertex2f(11.054749784201, -3.287812219938);  // N₅
    glVertex2f(11.570003583529, -3.744947910083);  // K₅
    glVertex2f(12.091008932919, -4.219336937975);  // V₄
    
    glEnd();

    //triangulo 237
    glBegin(GL_TRIANGLES);
    glColor3ub(110, 48, 17);
    glVertex2f(10.431191717232, -3.914573528159);  // F₄
    glVertex2f(11.424826833770, -4.567194783107);  // L₄
    glVertex2f(10.837877302811, -5.056319392240);  // K₄
    glEnd();

    //triangulo 238
    glBegin(GL_TRIANGLES);
    glColor3ub(123, 52, 14);
    glVertex2f(10.431191717232, -3.914573528159);  // F₄
    glVertex2f(10.837877302811, -5.056319392240);  // K₄
    glVertex2f(10.108119692108, -4.566687917255);  // G₄
    glEnd();

    //triangulo 239
    glBegin(GL_TRIANGLES);
    glColor3ub(134, 57, 13);
    glVertex2f(10.108119692108, -4.566687917255);  // G₄
    glVertex2f(10.837877302811, -5.056319392240);  // K₄
    glVertex2f(10.429314394007, -5.666286551864);  // J₄
    glEnd();

    //triangulo 240
    glBegin(GL_TRIANGLES);
    glColor3ub(142, 60, 11);
    glVertex2f(10.108119692108, -4.566687917255);  // G₄
    glVertex2f(10.429314394007, -5.666286551864);  // J₄
    glVertex2f(9.577662133400, -5.395829415049);  // H₄
    glEnd();

    //triangulo 241
    glBegin(GL_POLYGON);
    glColor3ub(138, 75, 13);
    glVertex2f(6.432264481184, -6.684504342836);  // W₂
    glVertex2f(7.560607773458, -6.369909035627);  // I₃
    glVertex2f(8.259088913341, -6.590280521370);  // O₃
    glVertex2f(7.463209191119, -6.967986148459);  // N₃
    glEnd();

    //triangulo 242
    glBegin(GL_TRIANGLES);
    glColor3ub(154, 74, 11);
    glVertex2f(7.560607773458, -6.369909035627);  // I₃
    glVertex2f(8.626677425418, -5.710091515387);  // M₃
    glVertex2f(8.259088913341, -6.590280521370);  // O₃
    glEnd();

    //triangulo 243
    glBegin(GL_POLYGON);
    glColor3ub(118, 69, 12);
    glVertex2f(7.463209191119, -6.967986148459);  // N₃
    glVertex2f(8.259088913341, -6.590280521370);  // O₃
    glVertex2f(9.065335719771, -6.863984829494);  // N₄
    glVertex2f(8.236670394692, -6.955350268120);  // E₁₂
    glEnd();

    //triangulo 244
    glBegin(GL_TRIANGLES);
    glColor3ub(122, 68, 14);
    glVertex2f(8.259088913341, -6.590280521370);  // O₃
    glVertex2f(8.934357095546, -6.157635802783);  // M₄
    glVertex2f(9.065335719771, -6.863984829494);  // N₄
    glEnd();

    //triangulo 245
    glBegin(GL_TRIANGLES);
    glColor3ub(147, 70, 10);
    glVertex2f(8.259088913341, -6.590280521370);  // O₃
    glVertex2f(8.626677425418, -5.710091515387);  // M₃
    glVertex2f(8.934357095546, -6.157635802783);  // M₄
    glEnd();

    //triangulo 246
    glBegin(GL_TRIANGLES);
    glColor3ub(119, 65, 14);
    glVertex2f(8.934357095546, -6.157635802783);  // M₄
    glVertex2f(9.710013498224, -6.368324226148);  // I₄
    glVertex2f(9.065335719771, -6.863984829494);  // N₄
    glEnd();

    //triangulo 247
    glBegin(GL_TRIANGLES);
    glColor3ub(139, 66, 11);
    glVertex2f(8.934357095546, -6.157635802783);  // M₄
    glVertex2f(9.577662133400, -5.395829415049);  // H₄
    glVertex2f(9.710013498224, -6.368324226148);  // I₄
    glEnd();

    //triangulo 248
    glBegin(GL_TRIANGLES);
    glColor3ub(153, 69, 10);
    glVertex2f(8.626677425418, -5.710091515387);  // M₃
    glVertex2f(9.577662133400, -5.395829415049);  // H₄
    glVertex2f(8.934357095546, -6.157635802783);  // M₄
    glEnd();

    //triangulo 249
    glBegin(GL_TRIANGLES);
    glColor3ub(126, 60, 13);
    glVertex2f(9.577662133400, -5.395829415049);  // H₄
    glVertex2f(10.429314394007, -5.666286551864);  // J₄
    glVertex2f(9.710013498224, -6.368324226148);  // I₄
    glEnd();

    //triangulo 250
    glBegin(GL_TRIANGLES);
    glColor3ub(100, 57, 15);
    glVertex2f(9.710013498224, -6.368324226148);  // I₄
    glVertex2f(10.429314394007, -5.666286551864);  // J₄
    glVertex2f(10.833614766336, -6.686981488709);  // P₄
    glEnd();

    //triangulo 251
    glBegin(GL_TRIANGLES);
    glColor3ub(103, 61, 14);
    glVertex2f(9.710013498224, -6.368324226148);  // I₄
    glVertex2f(10.099131289551, -7.055774957824);  // O₄
    glVertex2f(9.065335719771, -6.863984829494);  // N₄
    glEnd();

    //triangulo 252
    glBegin(GL_TRIANGLES);
    glColor3ub(94, 58, 16);
    glVertex2f(9.710013498224, -6.368324226148);  // I₄
    glVertex2f(10.833614766336, -6.686981488709);  // P₄
    glVertex2f(10.099131289551, -7.055774957824);  // O₄
    glEnd();

    //triangulo 253
    glBegin(GL_POLYGON);
    glColor3ub(90, 58, 14);
    glVertex2f(10.099131289551, -7.055774957824);  // O₄
    glVertex2f(10.833614766336, -6.686981488709);  // P₄
    glVertex2f(11.663760839244, -6.814230156819);  // R₄
    glVertex2f(11.141902892785, -6.939105465800);  // L₁₂
    glEnd();

    //triangulo 254
    glBegin(GL_TRIANGLES);
    glColor3ub(101, 54, 18);
    glVertex2f(10.429314394007, -5.666286551864);  // J₄
    glVertex2f(11.445620265341, -6.105273291635);  // Q₄
    glVertex2f(10.833614766336, -6.686981488709);  // P₄
    glEnd();

    //triangulo 255
    glBegin(GL_TRIANGLES);
    glColor3ub(86, 55, 17);
    glVertex2f(11.445620265341, -6.105273291635);  // Q₄
    glVertex2f(11.663760839244, -6.814230156819);  // R₄
    glVertex2f(10.833614766336, -6.686981488709);  // P₄
    glEnd();

    //triangulo 256
    glBegin(GL_TRIANGLES);
    glColor3ub(112, 52, 13);
    glVertex2f(10.429314394007, -5.666286551864);  // J₄
    glVertex2f(10.837877302811, -5.056319392240);  // K₄
    glVertex2f(11.445620265341, -6.105273291635);  // Q₄
    glEnd();

    //triangulo 257
    glBegin(GL_TRIANGLES);
    glColor3ub(96, 50, 19);
    glVertex2f(10.837877302811, -5.056319392240);  // K₄
    glVertex2f(12.145406903, -5.603384151143);  // S₄
    glVertex2f(11.445620265341, -6.105273291635);  // Q₄
    glEnd();

    //triangulo 258
    glBegin(GL_TRIANGLES);
    glColor3ub(81, 49, 17);
    glVertex2f(11.445620265341, -6.105273291635);  // Q₄
    glVertex2f(12.145406903, -5.603384151143);  // S₄
    glVertex2f(12.550173321199, -6.634481498245);  // D₅
    glEnd();

    //triangulo 259
    glBegin(GL_TRIANGLES);
    glColor3ub(77, 50, 15);
    glVertex2f(11.445620265341, -6.105273291635);  // Q₄
    glVertex2f(12.550173321199, -6.634481498245);  // D₅
    glVertex2f(11.663760839244, -6.814230156819);  // R₄
    glEnd();

    //triangulo 260
    glBegin(GL_TRIANGLES);
    glColor3ub(96, 47, 18);
    glVertex2f(10.837877302811, -5.056319392240);  // K₄
    glVertex2f(11.424826833770, -4.567194783107);  // L₄ 
    glVertex2f(12.145406903, -5.603384151143);  // S₄
    glEnd();

    //triangulo 261
    glBegin(GL_TRIANGLES);
    glColor3ub(98, 48, 17);
    glVertex2f(11.424826833770, -4.567194783107);  // L₄
    glVertex2f(11.898854806335, -4.678813787601);  // T₄
    glVertex2f(12.145406903, -5.603384151143);  // S₄
    glEnd();

    //triangulo 262
    glBegin(GL_TRIANGLES);
    glColor3ub(90, 48, 19);
    glVertex2f(11.898854806335, -4.678813787601);  // T₄
    glVertex2f(12.843971177956, -5.249992812189);  // U₄
    glVertex2f(12.145406903, -5.603384151143);  // S₄
    glEnd();

    //triangulo 263
    glBegin(GL_TRIANGLES);
    glColor3ub(84, 46, 18);
    glVertex2f(12.145406903, -5.603384151143);  // S₄
    glVertex2f(12.843971177956, -5.249992812189);  // U₄
    glVertex2f(13.467933841151, -6.257904985482);  // E₅
    glEnd();

    //triangulo 264
    glBegin(GL_POLYGON);
    glColor3ub(74, 46, 17);
    glVertex2f(12.145406903, -5.603384151143);  // S₄
    glVertex2f(13.467933841151, -6.257904985482);  // E₅
    glVertex2f(13.814531402294, -6.369325280424);  // C₅
    glVertex2f(12.550173321199, -6.634481498245);  // D₅
    glEnd();

    //triangulo 265
    glBegin(GL_TRIANGLES);
    glColor3ub(83, 46, 20);
    glVertex2f(12.843971177956, -5.249992812189);  // U₄
    glVertex2f(13.903738923759, -5.719170470871);  // B₅
    glVertex2f(13.467933841151, -6.257904985482);  // E₅
    glEnd();

    //triangulo 266
    glBegin(GL_TRIANGLES);
    glColor3ub(107, 50, 16);
    glVertex2f(12.091008932919, -4.219336937975);  // V₄
    glVertex2f(13.143942895905, -4.736342610222);  // W₄
    glVertex2f(12.843971177956, -5.249992812189);  // U₄
    glEnd();

    //triangulo 267
    glBegin(GL_TRIANGLES);
    glColor3ub(95, 48, 19);
    glVertex2f(12.843971177956, -5.249992812189);  // U₄
    glVertex2f(13.143942895905, -4.736342610222);  // W₄
    glVertex2f(13.903738923759, -5.719170470871);  // B₅
    glEnd();

    //triangulo 268
    glBegin(GL_TRIANGLES);
    glColor3ub(96, 46, 19);
    glVertex2f(13.143942895905, -4.736342610222);  // W₄
    glVertex2f(14.251204879155, -5.263403438464);  // A₅
    glVertex2f(13.903738923759, -5.719170470871);  // B₅
    glEnd();

    //triangulo 269
    glBegin(GL_POLYGON);
    glColor3ub(82, 47, 20);
    glVertex2f(13.467933841151, -6.257904985482);  // E₅
    glVertex2f(13.903738923759, -5.719170470871);  // B₅
    glVertex2f(14.945281218736, -6.082429735599);  // H₅
    glVertex2f(13.814531402294, -6.369325280424);  // C₅
    glEnd();

    //triangulo 270
    glBegin(GL_POLYGON);
    glColor3ub(97, 47, 19);
    glVertex2f(13.903738923759, -5.719170470871);  // B₅
    glVertex2f(14.251204879155, -5.263403438464);  // A₅
    glVertex2f(15.263216890277, -5.974741846852);  // F₅
    glVertex2f(14.945281218736, -6.082429735599);  // H₅
    glEnd();

    //triangulo 271
    glBegin(GL_POLYGON);
    glColor3ub(101, 48, 18);
    glVertex2f(11.424826833770, -4.567194783107);  // L₄
    glVertex2f(12.091008932919, -4.219336937975);  // V₄
    glVertex2f(12.843971177956, -5.249992812189);  // U₄
    glVertex2f(11.898854806335, -4.678813787601);  // T₄
    glEnd();
    
    //triangulo 272
    glBegin(GL_TRIANGLES);
    glColor3ub(203, 86, 3);
    glVertex2f(11.570003583529, -3.744947910083);  // K₅
    glVertex2f(12.218949144780, -3.696363750417);  // J₅
    glVertex2f(12.091008932919, -4.219336937975);  // V₄
    glEnd();

    //triangulo 273
    glBegin(GL_POLYGON);
    glColor3ub(231, 109, 1);
    glVertex2f(11.570003583529, -3.744947910083);  // K₅
    glVertex2f(12.029824904335, -3.333316499382);  // D₇
    glVertex2f(12.579800451558, -2.870433036097);  // L₅
    glVertex2f(12.218949144780, -3.696363750417);  // J₅
    glEnd();

    //triangulo 274
    glBegin(GL_TRIANGLES);
    glColor3ub(172, 69, 6);
    glVertex2f(12.218949144780, -3.696363750417);  // J₅
    glVertex2f(12.579800451558, -2.870433036097);  // L₅
    glVertex2f(12.919236548009, -3.848512073512);  // Z₄
    glEnd();

    //triangulo 275
    glBegin(GL_TRIANGLES);
    glColor3ub(155, 65, 9);
    glVertex2f(12.579800451558, -2.870433036097);  // L₅
    glVertex2f(13.207983823693, -3.363215226994);  // M₅
    glVertex2f(12.919236548009, -3.848512073512);  // Z₄
    glEnd();

    //triangulo 276
    glBegin(GL_TRIANGLES);
    glColor3ub(162, 64, 7);
    glVertex2f(12.218949144780, -3.696363750417);  // J₅
    glVertex2f(12.919236548009, -3.848512073512);  // Z₄
    glVertex2f(12.091008932919, -4.219336937975);  // V₄
    glEnd();

    //triangulo 277
    glBegin(GL_TRIANGLES);
    glColor3ub(135, 56, 16);
    glVertex2f(12.091008932919, -4.219336937975);  // V₄
    glVertex2f(12.919236548009, -3.848512073512);  // Z₄
    glVertex2f(13.143942895905, -4.736342610222);  // W₄
    glEnd();

    //triangulo 278
    glBegin(GL_TRIANGLES);
    glColor3ub(117, 50, 15);
    glVertex2f(12.919236548009, -3.848512073512);  // Z₄
    glVertex2f(13.686145257348, -4.292445339139);  // I₅
    glVertex2f(13.143942895905, -4.736342610222);  // W₄
    glEnd();

    //triangulo 279
    glBegin(GL_TRIANGLES);
    glColor3ub(131, 56, 11);
    glVertex2f(12.919236548009, -3.848512073512);  // Z₄
    glVertex2f(13.207983823693, -3.363215226994);  // M₅
    glVertex2f(13.686145257348, -4.292445339139);  // I₅
    glEnd();

    //triangulo 280
    glBegin(GL_TRIANGLES);
    glColor3ub(135, 60, 9);
    glVertex2f(13.207983823693, -3.363215226994);  // M₅
    glVertex2f(13.8431509098699, -4.0042596750649); //I13
    glVertex2f(13.686145257348, -4.292445339139);  // I₅
    glEnd();

    //triangulo 281
    glBegin(GL_POLYGON);
    glColor3ub(130, 55, 12);
    glVertex2f(13.686145257348, -4.292445339139);  // I₅
    glVertex2f(13.8431509098699, -4.0042596750649); //I13
    glVertex2f(14.525075386954, -4.307067197443);  // R₅
    glVertex2f(14.832465335289, -4.872222985869);  // G₅
    glEnd();

    //triangulo 282
    glBegin(GL_TRIANGLES);
    glColor3ub(108, 49, 17);
    glVertex2f(13.143942895905, -4.736342610222);  // W₄
    glVertex2f(13.686145257348, -4.292445339139);  // I₅
    glVertex2f(14.251204879155, -5.263403438464);  // A₅
    glEnd();

    //triangulo 283
    glBegin(GL_TRIANGLES);
    glColor3ub(118, 51, 15);
    glVertex2f(13.686145257348, -4.292445339139);  // I₅
    glVertex2f(14.832465335289, -4.872222985869);  // G₅
    glVertex2f(14.251204879155, -5.263403438464);  // A₅
    glEnd();

    //triangulo 284
    glBegin(GL_TRIANGLES);
    glColor3ub(108, 51, 15);
    glVertex2f(14.251204879155, -5.263403438464);  // A₅
    glVertex2f(14.832465335289, -4.872222985869);  // G₅
    glVertex2f(15.263216890277, -5.974741846852);  // F₅
    glEnd();

    //triangulo 285
    glBegin(GL_TRIANGLES);
    glColor3ub(138, 61, 12);
    glVertex2f(14.525075386954, -4.307067197443);  // R₅
    glVertex2f(15.313985518934, -4.768119536568);  // V₅
    glVertex2f(14.832465335289, -4.872222985869);  // G₅
    glEnd();

    //triangulo 286
    glBegin(GL_POLYGON);
    glColor3ub(127, 60, 11);
    glVertex2f(14.832465335289, -4.872222985869);  // G₅
    glVertex2f(15.313985518934, -4.768119536568);  // V₅
    glVertex2f(16.55, -5.364559208130);  // Q₅
    glVertex2f(16.340592644191, -5.509613421986);  // P₅
    glEnd();

    //triangulo 287
    glBegin(GL_POLYGON);
    glColor3ub(114, 56, 14);
    glVertex2f(14.832465335289, -4.872222985869);  // G₅
    glVertex2f(16.340592644191, -5.509613421986);  // P₅
    glVertex2f(16.2, -5.8);             // O₅
    glVertex2f(15.263216890277, -5.974741846852);  // F₅
    glEnd();

    //triangulo 288
    glBegin(GL_POLYGON);
    glColor3ub(46, 57, 15);
    glVertex2f(16.2, -5.8);             // O₅
    glVertex2f(16.340592644191, -5.509613421986);  // P₅
    glVertex2f(16.443534344365, -5.364559208130);  // Q₅
    glVertex2f(16.55, -5.2276133272334); //P13
    glVertex2f(16.55, -6.139655603135);  // A₁₃
    glVertex2f(15.655786096901, -6.701240662024);  // W₁₂
    glEnd();

    //triangulo 289
    glBegin(GL_POLYGON);
    glColor3ub(40, 56, 18);
    glVertex2f(15.655786096901, -6.701240662024);  // W₁₂
    glVertex2f(16.55, -6.139655603135);  // A₁₃
    glVertex2f(16.55, -6.6006892564886); //Q13
    glVertex2f(15.912304316456, -6.857163501136);  // Z₁₂
    glEnd();

    //triangulo 290
    glBegin(GL_POLYGON);
    glColor3ub(63, 65, 11);
    glVertex2f(14.319991122714, -6.682379096303);  // R₁₂
    glVertex2f(15.263216890277, -5.974741846852);  // F₅
    glVertex2f(16.2, -5.8);             // O₅
    glVertex2f(15.655786096901, -6.701240662024);  // W₁₂
    glEnd();

    //triangulo 291
    glBegin(GL_POLYGON);
    glColor3ub(27, 50, 19);
    glVertex2f(16.5355823198962, -6.6006892564886); //Q13
    glVertex2f(15.912304316456, -6.857163501136);  // Z₁₂
    glVertex2f(16.025559717371, -8.243393610038);  // E₁₃
    glVertex2f(16.488288685196, -9.101766295765);  // B₁₃
    glEnd();

    //triangulo 292
    glBegin(GL_TRIANGLES);
    glColor3ub(58, 64, 11);
    glVertex2f(14.319991122714, -6.682379096303);  // R₁₂
    glVertex2f(15.655786096901, -6.701240662024);  // W₁₂
    glVertex2f(14.319991122714, -7.303027749331);  // Q₁₂
    glEnd();

    //triangulo 293
    glBegin(GL_TRIANGLES);
    glColor3ub(46, 59, 12);
    glVertex2f(14.319991122714, -7.303027749331);  // Q₁₂
    glVertex2f(15.655786096901, -6.701240662024);  // W₁₂
    glVertex2f(15.138719130992, -7.443884128248);  // V₁₂
    glEnd();

    //triangulo 294
    glBegin(GL_POLYGON);
    glColor3ub(55, 65, 10);
    glVertex2f(14.319991122714, -7.303027749331);  // Q₁₂
    glVertex2f(15.138719130992, -7.443884128248);  // V₁₂
    glVertex2f(15.072692680642, -7.888462294387);  // U₁₂
    glVertex2f(14.091099420888, -7.936881692850);  // S₁₂
    glVertex2f(13.985457096962, -7.452687700281);  // P₁₂
    glEnd();

    //triangulo 295
    glBegin(GL_TRIANGLES);
    glColor3ub(39, 59, 13);
    glVertex2f(14.091099420888, -7.936881692850);  // S₁₂
    glVertex2f(15.072692680642, -7.888462294387);  // U₁₂
    glVertex2f(14.416711705558, -8.580129255982);  // D₁₃
    glEnd();

    //triangulo 296
    glBegin(GL_TRIANGLES);
    glColor3ub(47, 63, 12);
    glVertex2f(13.080737086123, -8.172926897853);  // H₁₀
    glVertex2f(14.091099420888, -7.936881692850);  // S₁₂
    glVertex2f(14.416711705558, -8.580129255982);  // D₁₃
    glVertex2f(13.815783940115, -8.772082582346);  // M₁₀
    glEnd();

    //triangulo 297
    glBegin(GL_TRIANGLES);
    glColor3ub(50, 63, 12);
    glVertex2f(13.061086726270, -7.769614679977);  // O₁₂
    glVertex2f(14.091099420888, -7.936881692850);  // S₁₂
    glVertex2f(13.080737086123, -8.172926897853);  // H₁₀
    glEnd();

    //triangulo 298
    glBegin(GL_TRIANGLES);
    glColor3ub(64, 68, 10);
    glVertex2f(12.082385271265, -7.905213439538);  // F₁₀
    glVertex2f(13.061086726270, -7.769614679977);  // O₁₂
    glVertex2f(13.080737086123, -8.172926897853);  // H₁₀
    glEnd();

    //triangulo 299
    glBegin(GL_TRIANGLES);
    glColor3ub(62, 68, 10);
    glVertex2f(13.061086726270, -7.769614679977);  // O₁₂
    glVertex2f(13.985457096962, -7.452687700281);  // P₁₂
    glVertex2f(14.091099420888, -7.936881692850);  // S₁₂
    glEnd();

    //triangulo 300
    glBegin(GL_POLYGON);
    glColor3ub(83, 76, 7);
    glVertex2f(11.734995761085, -7.527754833014);  // U₁₁
    glVertex2f(12.849802114830, -7.263411877861);  // N₁₂
    glVertex2f(13.985457096962, -7.452687700281);  // P₁₂
    glVertex2f(13.061086726270, -7.769614679977);  // O₁₂
    glEnd();

    //triangulo 301
    glBegin(GL_POLYGON);
    glColor3ub(69, 68, 9);
    glVertex2f(12.849802114830, -7.263411877861);  // N₁₂
    glVertex2f(14.319991122714, -6.682379096303);  // R₁₂
    glVertex2f(14.319991122714, -7.303027749331);  // Q₁₂
    glVertex2f(13.985457096962, -7.452687700281);  // P₁₂
    glEnd();

    //triangulo 302
    glBegin(GL_POLYGON);
    glColor3ub(80, 70, 9);
    glVertex2f(12.642919230487, -6.854047872672);  // M₁₂
    glVertex2f(13.814531402294, -6.369325280424);  // C₅
    glVertex2f(15.263216890277, -5.974741846852);  // F₅
    glVertex2f(14.319991122714, -6.682379096303);  // R₁₂
    glEnd();

    //triangulo 303
    glBegin(GL_POLYGON);
    glColor3ub(89, 75, 8);
    glVertex2f(11.511660017710, -7.241634022556);  // K₁₂
    glVertex2f(12.642919230487, -6.854047872672);  // M₁₂
    glVertex2f(14.319991122714, -6.682379096303);  // R₁₂
    glVertex2f(12.849802114830, -7.263411877861);  // N₁₂
    glEnd();

    //triangulo 304
    glBegin(GL_POLYGON);
    glColor3ub(99, 80, 5);
    glVertex2f(10.467337249499, -7.748476550359);  // V₁₁
    glVertex2f(10.619014029255, -7.663680033834);  // J₁₂
    glVertex2f(11.511660017710, -7.241634022556);  // K₁₂
    glVertex2f(12.849802114830, -7.263411877861);  // N₁₂
    glVertex2f(11.734995761085, -7.527754833014);  // U₁₁
    glEnd();

    //triangulo 305
    glBegin(GL_POLYGON);
    glColor3ub(120, 90, 3);
    glVertex2f(9.162395052279, -7.488138772506);  // H₁₂
    glVertex2f(11.141902892785, -6.939105465800);  // L₁₂
    glVertex2f(12.642919230487, -6.854047872672);  // M₁₂
    glVertex2f(11.511660017710, -7.241634022556);  // K₁₂
    glVertex2f(10.465882290650, -7.320067352086);  // I₁₂
    glEnd();

    //triangulo 306
    glBegin(GL_POLYGON);
    glColor3ub(97, 74, 7);
    glVertex2f(11.141902892785, -6.939105465800);  // L₁₂
    glVertex2f(11.663760839244, -6.814230156819);  // R₄
    glVertex2f(12.550173321199, -6.634481498245);  // D₅
    glVertex2f(13.814531402294, -6.369325280424);  // C₅
    glVertex2f(12.642919230487, -6.854047872672);  // M₁₂
    glEnd();

    //triangulo 307
    glBegin(GL_POLYGON);
    glColor3ub(100, 87, 7);
    glVertex2f(11.734995761085, -7.527754833014);  // U₁₁
    glVertex2f(13.061086726270, -7.769614679977);  // O₁₂
    glVertex2f(12.082385271265, -7.905213439538);  // F₁₀
    glVertex2f(10.516733918002, -8.047657120409);  // S₁₁
    glEnd();

    //triangulo 308
    glBegin(GL_TRIANGLES);
    glColor3ub(84, 77, 8);
    glVertex2f(10.516733918002, -8.047657120409);  // S₁₁
    glVertex2f(12.082385271265, -7.905213439538);  // F₁₀
    glVertex2f(11.2, -8.4);             // G₁₀
    glEnd();

    //triangulo 309
    glBegin(GL_TRIANGLES);
    glColor3ub(96, 83, 6);
    glVertex2f(9.799226518613, -8.552569734794);  // R₁₁
    glVertex2f(10.516733918002, -8.047657120409);  // S₁₁
    glVertex2f(11.2, -8.4);             // G₁₀
    glEnd();

    //triangulo 310
    glBegin(GL_TRIANGLES);
    glColor3ub(76, 70, 7);
    glVertex2f(9.799226518613, -8.552569734794);  // R₁₁
    glVertex2f(11.2, -8.4);             // G₁₀
    glVertex2f(10.799561000230, -8.815531895257);  // T₁₁
    glEnd();

    //triangulo 311
    glBegin(GL_POLYGON);
    glColor3ub(65, 59, 10);
    glVertex2f(9.364770272665, -8.767654900765);  // Q₁₁
    glVertex2f(9.799226518613, -8.552569734794);  // R₁₁
    glVertex2f(10.799561000230, -8.815531895257);  // T₁₁
    glVertex2f(10.588365429077, -9.057441006509);  // Q₁₀
    glVertex2f(9.757274213096, -8.898074935464);  // T₉
    glEnd();

    //triangulo 312
    glBegin(GL_POLYGON);
    glColor3ub(85, 70, 7);
    glVertex2f(8.898055325417, -8.596068523104);  // P₁₁
    glVertex2f(9.388792365528, -8.448504238315);  // O₁₁
    glVertex2f(9.799226518613, -8.552569734794);  // R₁₁
    glVertex2f(9.364770272665, -8.767654900765);  // Q₁₁
    glEnd();

    //triangulo 313
    glBegin(GL_POLYGON);
    glColor3ub(91, 75, 3);
    glVertex2f(8.785629260726, -8.561632252841);  // O₉
    glVertex2f(8.980416786695, -8.362711094984);  // N₁₁
    glVertex2f(9.388792365528, -8.448504238315);  // O₁₁
    glVertex2f(8.898055325417, -8.596068523104);  // P₁₁
    glEnd();

    //triangulo 314
    glBegin(GL_POLYGON);
    glColor3ub(104, 88, 55);
    glVertex2f(8.980416786695, -8.362711094984);  // N₁₁
    glVertex2f(10.516733918002, -8.047657120409);  // S₁₁
    glVertex2f(9.799226518613, -8.552569734794);  // R₁₁
    glVertex2f(9.388792365528, -8.448504238315);  // O₁₁
    glEnd();

    //triangulo 315
    glBegin(GL_POLYGON);
    glColor3ub(135, 107, 3);
    glVertex2f(8.980416786695, -8.362711094984);  // N₁₁
    glVertex2f(9.495175019678, -8.132785303418);  // M₁₁
    glVertex2f(10.467337249499, -7.748476550359);  // V₁₁
    glVertex2f(11.734995761085, -7.527754833014);  // U₁₁
    glVertex2f(10.516733918002, -8.047657120409);  // S₁₁
    glVertex2f(8.980416786695, -8.362711094984);  // N₁₁
    glEnd();

    //triangulo 316
    glBegin(GL_TRIANGLES);
    glColor3ub(111, 85, 4);
    glVertex2f(10.467337249499, -7.748476550359);  // V₁₁
    glVertex2f(10.465882290650, -7.320067352086);  // I₁₂
    glVertex2f(11.511660017710, -7.241634022556);  // K₁₂
    glEnd();

    //triangulo 317
    glBegin(GL_TRIANGLES);
    glColor3ub(119, 87, 1);
    glVertex2f(9.162395052279, -7.488138772506);  // H₁₂
    glVertex2f(10.465882290650, -7.320067352086);  // I₁₂
    glVertex2f(10.467337249499, -7.748476550359);  // V₁₁
    glEnd();

    //triangulo 318
    glBegin(GL_TRIANGLES);
    glColor3ub(118, 87, 6);
    glVertex2f(10.099131289551, -7.055774957824);  // O₄
    glVertex2f(11.141902892785, -6.939105465800);  // L₁₂
    glVertex2f(9.162395052279, -7.488138772506);  // H₁₂
    glEnd();

    //triangulo TETA
    glBegin(GL_TRIANGLES);
    glColor3ub(134, 95, 5);
    glVertex2f(9.334201393153, -7.159465772573);  // F₁₂
    glVertex2f(10.099131289551, -7.055774957824);  // O₄
    glVertex2f(9.162395052279, -7.488138772506);  // H₁₂
    glEnd();

    //triangulo 319
    glBegin(GL_TRIANGLES);
    glColor3ub(137, 97, 3);
    glVertex2f(8.153066947219, -7.371261009027);  // A₁₂
    glVertex2f(9.334201393153, -7.159465772573);  // F₁₂
    glVertex2f(9.162395052279, -7.488138772506);  // H₁₂
    glEnd();

    //triangulo 320
    glBegin(GL_POLYGON);
    glColor3ub(104, 74, 9);
    glVertex2f(8.721284054020, -7.054627039401);  // D₁₂
    glVertex2f(9.065335719771, -6.863984829494);  // N₄
    glVertex2f(10.099131289551, -7.055774957824);  // O₄
    glVertex2f(9.334201393153, -7.159465772573);  // F₁₂
    glEnd();

    //triangulo 321
    glBegin(GL_POLYGON);
    glColor3ub(119, 86, 7);
    glVertex2f(8.721284054020, -7.054627039401);  // D₁₂
    glVertex2f(9.334201393153, -7.159465772573);  // F₁₂
    glVertex2f(8.153066947219, -7.371261009027);  // A₁₂
    glVertex2f(7.425783926996, -7.192293722142);  // C₁₂
    glEnd();

    //triangulo 322
    glBegin(GL_POLYGON);
    glColor3ub(134, 90, 4);
    glVertex2f(6.8, -7);                // B₁₂
    glVertex2f(7.463209191119, -6.967986148459);  // N₃
    glVertex2f(8.236670394692, -6.955350268120);  // E₁₂
    glVertex2f(8.721284054020, -7.054627039401);  // D₁₂
    glVertex2f(7.425783926996, -7.192293722142);  // C₁₂
    glEnd();

    //triangulo 323
    glBegin(GL_TRIANGLES);
    glColor3ub(134, 87, 8);
    glVertex2f(6.432264481184, -6.684504342836);  // W₂
    glVertex2f(7.463209191119, -6.967986148459);  // N₃
    glVertex2f(6.8, -7);                          // B₁₂
    glEnd();

    //triangulo 324
    glBegin(GL_TRIANGLES);
    glColor3ub(119, 81, 8);
    glVertex2f(8.236670394692, -6.955350268120);  // E₁₂
    glVertex2f(9.065335719771, -6.863984829494);  // N₄
    glVertex2f(8.721284054020, -7.054627039401);  // D₁₂
    glEnd();

    //triangulo 325
    glBegin(GL_POLYGON);
    glColor3ub(247, 151, 0);
    glVertex2f(8.153066947219, -7.371261009027);  // A₁₂
    glVertex2f(9.162395052279, -7.488138772506);  // H₁₂
    glVertex2f(10.467337249499, -7.748476550359);  // V₁₁
    glVertex2f(9.624515555343, -7.783925047148);  // Z₁₁
    glEnd();

    //triangulo 326
    glBegin(GL_POLYGON);
    glColor3ub(249, 148, 0);
    glVertex2f(9.624515555343, -7.783925047148);  // Z₁₁
    glVertex2f(10.467337249499, -7.748476550359);  // V₁₁
    glVertex2f(9.495175019678, -8.132785303418);  // M₁₁
    glVertex2f(8.944806945511, -7.991564491927);  // W₁₁
    glEnd();

    //triangulo 327
    glBegin(GL_TRIANGLES);
    glColor3ub(241, 129, 0);
    glVertex2f(7.906088258667, -8.309876343555);  // L₁₁
    glVertex2f(8.944806945511, -7.991564491927);  // W₁₁
    glVertex2f(9.495175019678, -8.132785303418);  // M₁₁
    glEnd();

    //triangulo 328
    glBegin(GL_TRIANGLES);
    glColor3ub(251, 148, 1);
    glVertex2f(7.906088258667, -8.309876343555);  // L₁₁
    glVertex2f(9.495175019678, -8.132785303418);  // M₁₁
    glVertex2f(8.980416786695, -8.362711094984);  // N₁₁
    glEnd();

    //triangulo 329
    glBegin(GL_TRIANGLES);
    glColor3ub(235, 128, 2);
    glVertex2f(7.906088258667, -8.309876343555);  // L₁₁
    glVertex2f(8.980416786695, -8.362711094984);  // N₁₁
    glVertex2f(6.989216683413, -8.587023243333);  // P₉
    glEnd();

    //triangulo 331
    glBegin(GL_TRIANGLES);
    glColor3ub(237, 126, 1);
    glVertex2f(6.989216683413, -8.587023243333);  // P₉
    glVertex2f(8.341286927115, -8.739369186285);  // M₉
    glVertex2f(7.554307189036, -9.006080975504);  // W₇
    glEnd();

    //triangulo 330
    glBegin(GL_POLYGON);
    glColor3ub(247, 144, 1);
    glVertex2f(6.989216683413, -8.587023243333);  // P₉
    glVertex2f(8.980416786695, -8.362711094984);  // N₁₁
    glVertex2f(8.785629260726, -8.561632252841);  // O₉
    glVertex2f(8.341286927115, -8.739369186285);  // M₉
    glEnd();

    //triangulo 332
    glBegin(GL_TRIANGLES);
    glColor3ub(237, 128, 0);
    glVertex2f(8.341286927115, -8.739369186285);  // M₉
    glVertex2f(8.785629260726, -8.561632252841);  // O₉
    glVertex2f(9.045886913270, -8.840933148253);  // N₉
    glEnd();

    //triangulo 333
    glBegin(GL_POLYGON);
    glColor3ub(226, 116, 0);
    glVertex2f(8.785629260726, -8.561632252841);  // O₉
    glVertex2f(8.898055325417, -8.596068523104);  // P₁₁
    glVertex2f(9.757274213096, -8.898074935464);  // T₉
    glVertex2f(10.507321899923, -9.212480801453);  // S₉
    glVertex2f(9.547358975488, -9.266232238995);  // L₉
    glVertex2f(9.045886913270, -8.840933148253);  // N₉
    glEnd();

    //triangulo 334
    glBegin(GL_TRIANGLES);
    glColor3ub(195, 92, 2);
    glVertex2f(9.757274213096, -8.898074935464);  // T₉
    glVertex2f(10.588365429077, -9.057441006509);  // Q₁₀
    glVertex2f(10.507321899923, -9.212480801453);  // S₉
    glEnd();

    //triangulo 335
    glBegin(GL_TRIANGLES);
    glColor3ub(211, 99, 1);
    glVertex2f(9.547358975488, -9.266232238995);  // L₉
    glVertex2f(10.507321899923, -9.212480801453);  // S₉
    glVertex2f(10.334663946441, -9.684674449692);  // R₉
    glEnd();

    //triangulo 336
    glBegin(GL_POLYGON);
    glColor3ub(244, 137, 0);
    glVertex2f(8.341286927115, -8.739369186285);  // M₉
    glVertex2f(9.045886913270, -8.840933148253);  // N₉
    glVertex2f(9.547358975488, -9.266232238995);  // L₉
    glVertex2f(9.191885108559, -9.405882686702);  // K₉
    glEnd();

    //triangulo 337
    glBegin(GL_TRIANGLES);
    glColor3ub(245, 138, 0);
    glVertex2f(7.554307189036, -9.006080975504);  // W₇
    glVertex2f(8.341286927115, -8.739369186285);  // M₉
    glVertex2f(9.191885108559, -9.405882686702);  // K₉
    glEnd();

    //triangulo 338
    glBegin(GL_POLYGON);
    glColor3ub(235, 122, 1);
    glVertex2f(7.554307189036, -9.006080975504);  // W₇
    glVertex2f(9.191885108559, -9.405882686702);  // K₉
    glVertex2f(8.222410257891, -9.666859319138);  // H₉
    glVertex2f(7.557220770283, -9.348239110872);  // D₉
    glEnd();

    //triangulo 339
    glBegin(GL_TRIANGLES);
    glColor3ub(225, 107, 0);
    glVertex2f(8.222410257891, -9.666859319138);  // H₉
    glVertex2f(9.191885108559, -9.405882686702);  // K₉
    glVertex2f(9.775877889916, -10.167612401463);  // J₉
    glEnd();

    //triangulo 340
    glBegin(GL_TRIANGLES);
    glColor3ub(219, 104, 1);
    glVertex2f(9.191885108559, -9.405882686702);  // K₉
    glVertex2f(10.334663946441, -9.684674449692);  // R₉
    glVertex2f(9.775877889916, -10.167612401463);  // J₉
    glEnd();

    //triangulo 341
    glBegin(GL_TRIANGLES);
    glColor3ub(223, 109, 1);
    glVertex2f(9.191885108559, -9.405882686702);  // K₉
    glVertex2f(9.547358975488, -9.266232238995);  // L₉
    glVertex2f(10.334663946441, -9.684674449692);  // R₉
    glEnd();

    //triangulo 342
    glBegin(GL_POLYGON);
    glColor3ub(217, 101, 1);
    glVertex2f(7.211387850210, -9.970832225150);  // G₉
    glVertex2f(8.222410257891, -9.666859319138);  // H₉
    glVertex2f(9.775877889916, -10.167612401463);  // J₉
    glVertex2f(8.411112150968, -10.161264653840);  // I₉
    glEnd();

    //triangulo 343
    glBegin(GL_TRIANGLES);
    glColor3ub(225, 112, 1);
    glVertex2f(7.211387850210, -9.970832225150);  // G₉
    glVertex2f(7.557220770283, -9.348239110872);  // D₉
    glVertex2f(8.222410257891, -9.666859319138);  // H₉
    glEnd();

    //triangulo 344
    glBegin(GL_POLYGON);
    glColor3ub(174, 85, 2);
    glVertex2f(7.554307189036, -9.006080975504);  // W₇
    glVertex2f(7.557220770283, -9.348239110872);  // D₉
    glVertex2f(6.735054394006, -9.681186367017);  // Z₇
    glVertex2f(7.246229620460, -9.393601749747);  // A₈
    glEnd();

    //triangulo 345
    glBegin(GL_POLYGON);
    glColor3ub(194, 91, 1);
    glVertex2f(6.735054394006, -9.681186367017);  // Z₇
    glVertex2f(7.557220770283, -9.348239110872);  // D₉
    glVertex2f(7.211387850210, -9.970832225150);  // G₉
    glVertex2f(6.830543032309, -9.884862619531);  // C₉
    glEnd();

    //triangulo 346
    glBegin(GL_POLYGON);
    glColor3ub(149, 66, 1);
    glVertex2f(5.888614139146, -10.004551845711);  // W₈
    glVertex2f(6.735054394006, -9.681186367017);  // Z₇
    glVertex2f(6.830543032309, -9.884862619531);  // C₉
    glVertex2f(6.204482272208, -10.091686263493);  // E₉
    glEnd();

    //triangulo 347
    glBegin(GL_POLYGON);
    glColor3ub(167, 73, 1);
    glVertex2f(6.204482272208, -10.091686263493);  // E₉
    glVertex2f(6.830543032309, -9.884862619531);  // C₉
    glVertex2f(6.947929424828, -10.130815060999);  // F₉
    glVertex2f(6.531323065378, -10.207434858967);  // B₈
    glEnd();

    //triangulo 348
    glBegin(GL_TRIANGLES);
    glColor3ub(174, 77, 1);
    glVertex2f(6.531323065378, -10.207434858967);  // B₈
    glVertex2f(6.947929424828, -10.130815060999);  // F₉
    glVertex2f(7.115624271284, -10.477384410341);  // B₉
    glEnd();

    //triangulo 349
    glBegin(GL_TRIANGLES);
    glColor3ub(190, 87, 3);
    glVertex2f(6.830543032309, -9.884862619531);  // C₉
    glVertex2f(7.211387850210, -9.970832225150);  // G₉
    glVertex2f(6.947929424828, -10.130815060999);  // F₉
    glEnd();

    //triangulo 350
    glBegin(GL_POLYGON);
    glColor3ub(201, 88, 2);
    glVertex2f(7.211387850210, -9.970832225150);  // G₉
    glVertex2f(8.411112150968, -10.161264653840);  // I₉
    glVertex2f(9.106724264, -11.009293641554);  // Q₉
    glVertex2f(7.115624271284, -10.477384410341);  // B₉
    glVertex2f(6.947929424828, -10.130815060999);  // F₉
    glEnd();

    //triangulo 351
    glBegin(GL_TRIANGLES);
    glColor3ub(205, 87, 0);
    glVertex2f(8.411112150968, -10.161264653840);  // I₉
    glVertex2f(9.775877889916, -10.167612401463);  // J₉
    glVertex2f(9.106724264, -11.009293641554);  // Q₉
    glEnd();

    //triangulo 352
    glBegin(GL_TRIANGLES);
    glColor3ub(200, 83, 0);
    glVertex2f(9.775877889916, -10.167612401463);  // J₉
    glVertex2f(10.616491128365, -11.039079959095);  // W₉
    glVertex2f(9.106724264, -11.009293641554);  // Q₉
    glEnd();

    //triangulo 353
    glBegin(GL_TRIANGLES);
    glColor3ub(195, 81, 1);
    glVertex2f(9.775877889916, -10.167612401463);  // J₉
    glVertex2f(10.334663946441, -9.684674449692);  // R₉
    glVertex2f(11.064511377041, -10.659046911766);  // U₉
    glEnd();

    //triangulo 354
    glBegin(GL_POLYGON);
    glColor3ub(195, 82, 1);
    glVertex2f(9.775877889916, -10.167612401463);  // J₉
    glVertex2f(11.064511377041, -10.659046911766);  // U₉
    glVertex2f(12.5, -11);              // V₉
    glVertex2f(10.616491128365, -11.039079959095);  // W₉
    glEnd();

    //triangulo 355
    glBegin(GL_TRIANGLES);
    glColor3ub(167, 70, 3);
    glVertex2f(10.334663946441, -9.684674449692);  // R₉
    glVertex2f(12.5, -11);              // V₉
    glVertex2f(11.064511377041, -10.659046911766);  // U₉
    glEnd();

    //triangulo 356
    glBegin(GL_POLYGON);
    glColor3ub(119, 56, 6);
    glVertex2f(10.334663946441, -9.684674449692);  // R₉
    glVertex2f(11.092244762645, -9.991203407877);  // A₁₀
    glVertex2f(13.373824313834, -11.012895401300);  // Z₉
    glVertex2f(12.5, -11);              // V₉
    glEnd();

    //triangulo 357
    glBegin(GL_TRIANGLES);
    glColor3ub(178, 78, 2);
    glVertex2f(7.115624271284, -10.477384410341);  // B₉
    glVertex2f(9.106724264, -11.009293641554);  // Q₉
    glVertex2f(8.253351317003, -11.0348);  // C₈
    glEnd();

    //suelo alabol derecho

    glBegin(GL_POLYGON);
    glColor3ub(16, 40, 16);
    glVertex2f(14.106902832200, -8.962769244036);  // C₁₃
    glVertex2f(14.416711705558, -8.580129255982);  // D₁₃
    glVertex2f(16.488288685196, -9.101766295765);  // B₁₃
    glVertex2f(16.533156910531, -9.516928677007);  // J₁₁
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(29, 50, 12);
    glVertex2f(14.217819780992, -9.510050780367);  // N₁₀
    glVertex2f(14.106902832200, -8.962769244036);  // C₁₃
    glVertex2f(16.533156910531, -9.516928677007);  // J₁₁
    glVertex2f(16.521687877422, -10.263885784354);  // H₁₁
    glEnd();

    
    //Pasto

    //triangulo 380 
    glBegin(GL_TRIANGLES);
    glColor3ub(47, 62, 11);
    glVertex2f(13.080737086123, -8.172926897853);  // H₁₀
    glVertex2f(14.416711705558, -8.580129255982);  // D₁₃
    glVertex2f(13.815783940115, -8.772082582346);  // M₁₀
    glEnd();

    //triangulo 381
    glBegin(GL_TRIANGLES);
    glColor3ub(33, 54, 13);
    glVertex2f(14.416711705558, -8.580129255982);  // D₁₃
    glVertex2f(14.217819780992, -9.510050780367);  // N₁₀
    glVertex2f(13.815783940115, -8.772082582346);  // M₁₀
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(31, 43, 12);
    glVertex2f(11.092244762645, -9.991203407877);  // A₁₀
    glVertex2f(12.866748565170, -10.236414911582);  // O₁₀
    glVertex2f(16.5396, -11.0348);      // B
    glVertex2f(13.373824313834, -11.012895401300);  // Z₉
    glEnd();

    //Sombra 383 R-1
    glBegin(GL_POLYGON);
    glColor3ub(17, 38, 14);
    glVertex2f(12.866748565170, -10.236414911582);  // O₁₀
    glVertex2f(14.217819780992, -9.510050780367);  // N₁₀
    glVertex2f(16.521687877422, -10.263885784354);  // H₁₁
    glVertex2f(16.5396, -11.0348);      // B
    glVertex2f(15, -11);               // G₁₁
    glEnd();

    // Sombra 2 R2
    
    glBegin(GL_POLYGON);
    glColor3ub(12, 36, 17);
    glVertex2f(14.561755474170, -10.264493793553);  // E₁₁
    glVertex2f(15.977680326258, -10.032930375817);  // C₁₁
    glVertex2f(16.521687877422, -10.263885784354);  // H₁₁
    glVertex2f(16.5396, -11.0348);      // B
    glEnd();



    glPushMatrix();  // Guardar la matriz actual
    glRotatef(-10.0f, 0.0f, 0.0f, 1.0f); // rotar
    // glPushMatrix();  // Guardar la matriz actual
    glTranslatef(-15.9f, 6.5f, 0.0f);  // mover al destino
    glScalef(1.5, 1.3, 1.0);  // escalar en Y

    // glPushMatrix();

    glPushMatrix();
    // Trasladar al centro, voltear, y devolver
    glTranslatef(14.8f, 0.0f, 0.0f); 
    glScalef(-1.0, 1.0, 1.0);      // voltear en X
    glTranslatef(-14.8f, 0.0f, 0.0f); 

    //Roca 2 
    
    //triangulo 384 - 1
    glBegin(GL_TRIANGLES);
    glColor3ub(44, 55, 35);
    glVertex2f(14.068269600788, -10.006362721322);  // R₁₀
    glVertex2f(14.369502668818, -9.580429449272);  // S₁₀
    glVertex2f(14.432689938055, -9.725455378012);  // T₁₀
    glEnd();

    //triangulo 385 - 2
    glBegin(GL_POLYGON);
    glColor3ub(73, 70, 40);
    glVertex2f(14.369502668818, -9.580429449272);  // S₁₀
    glVertex2f(14.713597281364, -9.300298317866);  // V₁₀
    glVertex2f(15.096997844530, -9.562225435278);  // U₁₀
    glVertex2f(14.432689938055, -9.725455378012);  // T₁₀
    glEnd();

    //triangulo 386 - 3
    glBegin(GL_TRIANGLES);
    glColor3ub(81, 73, 43);
    glVertex2f(14.713597281364, -9.300298317866);  // V₁₀
    glVertex2f(15.150142477048, -9.281318091969);  // Z₁₀
    glVertex2f(15.096997844530, -9.562225435278);  // U₁₀
    glEnd();

    //triangulo 387 - 4
    glBegin(GL_TRIANGLES);
    glColor3ub(39, 53, 43);
    glVertex2f(15.096997844530, -9.562225435278);  // U₁₀
    glVertex2f(15.150142477048, -9.281318091969);  // Z₁₀
    glVertex2f(15.647424395610, -9.486304531681);  // W₁₀
    glEnd();

    //triangulo 388 - 5
    glBegin(GL_TRIANGLES);
    glColor3ub(22, 43, 35);
    glVertex2f(15.096997844530, -9.562225435278);  // U₁₀
    glVertex2f(15.647424395610, -9.486304531681);  // W₁₀
    glVertex2f(15.662608576331, -10.040527127941);  // A₁₁
    glEnd();

    //triangulo 389 - 6
    glBegin(GL_TRIANGLES);
    glColor3ub(16, 39, 34);
    glVertex2f(15.647424395610, -9.486304531681);  // W₁₀
    glVertex2f(15.977680326258, -10.032930375817);  // C₁₁
    glVertex2f(15.662608576331, -10.040527127941);  // A₁₁
    glEnd();

    //triangulo 390 - 7
    glBegin(GL_TRIANGLES);
    glColor3ub(10,33, 28);
    glVertex2f(15.662608576331, -10.040527127941);  // A₁₁
    glVertex2f(15.977680326258, -10.032930375817);  // C₁₁
    glVertex2f(15.480398076967, -10.401151420028);  // D₁₁
    glEnd();

    //triangulo 391 - 8
    glBegin(GL_TRIANGLES);
    glColor3ub(17, 40, 32);
    glVertex2f(14.861643043379, -10.0595073538407);  // B₁₁
    glVertex2f(15.662608576331, -10.040527127941);  // A₁₁
    glVertex2f(15.480398076967, -10.401151420028);  // D₁₁
    glEnd();

    //triangulo 392 - 9
    glBegin(GL_TRIANGLES);
    glColor3ub(28, 44, 36);
    glVertex2f(15.096997844530, -9.562225435278);  // U₁₀
    glVertex2f(15.662608576331, -10.040527127941);  // A₁₁
    glVertex2f(14.861643043379, -10.0595073538407);  // B₁₁
    glEnd();

    //triangulo 393 - 10
    glBegin(GL_TRIANGLES);
    glColor3ub(12, 35, 28);
    glVertex2f(14.561755474170, -10.264493793553);  // E₁₁
    glVertex2f(14.861643043379, -10.0595073538407);  // B₁₁
    glVertex2f(15.480398076967, -10.401151420028);  // D₁₁
    glEnd();

    //triangulo 394 - 11
    glBegin(GL_TRIANGLES);
    glColor3ub(48, 55, 39);
    glVertex2f(14.432689938055, -9.725455378012);  // T₁₀
    glVertex2f(15.096997844530, -9.562225435278);  // U₁₀
    glVertex2f(14.861643043379, -10.0595073538407);  // B₁₁
    glEnd();

    //triangulo 395 - 12
    glBegin(GL_POLYGON);
    glColor3ub(29, 45, 32);
    glVertex2f(14.432689938055, -9.725455378012);  // T₁₀
    glVertex2f(14.861643043379, -10.0595073538407);  // B₁₁
    glVertex2f(14.561755474170, -10.264493793553);  // E₁₁
    glVertex2f(14.068269600788, -10.006362721322);  // R₁₀
    glEnd();


    glPopMatrix();   // Restaurar la matriz anterior
    glPopMatrix();   // Restaurar la matriz anterior


    glPushMatrix();  // Guardar la matriz actual
    glScalef(1.5, 1.3, 1.0);  // escalar en Y

    glPushMatrix();  // Guardar la matriz actual
    glTranslatef(-15.5f, 3.6f, 0.0f);  // mover al destino

    glPushMatrix();
    // Trasladar al centro, voltear, y devolver
    glTranslatef(14.8f, 0.0f, 0.0f); 
    glScalef(-1.0, 1.0, 1.0);      // voltear en X
    glTranslatef(-14.8f, 0.0f, 0.0f); 
    // Roca 1
        //triangulo 358 - 1
    glBegin(GL_POLYGON);
    glColor3ub(91, 74, 35);
    glVertex2f(10.334663946441, -9.684674449692);  // R₉
    glVertex2f(10.507321899923, -9.212480801453);  // S₉
    glVertex2f(10.588365429077, -9.057441006509);  // Q₁₀
    glVertex2f(10.926634072591, -9.476753179199);  // B₁₀
    glEnd();

    //triangulo 359 - 2
    glBegin(GL_TRIANGLES);
    glColor3ub(99, 79, 39);
    glVertex2f(10.588365429077, -9.057441006509);  // Q₁₀
    glVertex2f(11.357868149083, -8.702299484178);  // E₁₀
    glVertex2f(10.926634072591, -9.476753179199);  // B₁₀
    glEnd();

    //triangulo 360 - 3
    glBegin(GL_POLYGON);
    glColor3ub(136, 101, 38);
    glVertex2f(10.588365429077, -9.057441006509);  // Q₁₀
    glVertex2f(10.799561000230, -8.815531895257);  // T₁₁
    glVertex2f(11.2, -8.4);             // G₁₀
    glVertex2f(11.357868149083, -8.702299484178);  // E₁₀
    glEnd();

    //triangulo 361 - 4
    glBegin(GL_TRIANGLES);
    glColor3ub(138, 100, 42);
    glVertex2f(11.2, -8.4);             // G₁₀
    glVertex2f(12.082385271265, -7.905213439538);  // F₁₀
    glVertex2f(11.357868149083, -8.702299484178);  // E₁₀
    glEnd();

    //triangulo 362 - 5
    glBegin(GL_TRIANGLES);
    glColor3ub(131, 95, 43);
    glVertex2f(11.357868149083, -8.702299484178);  // E₁₀
    glVertex2f(12.082385271265, -7.905213439538);  // F₁₀
    glVertex2f(11.916774581212, -8.669841519149);  // D₁₀
    glEnd();

    //triangulo 363 - 6
    glBegin(GL_POLYGON);
    glColor3ub(77, 68, 43);
    glVertex2f(11.916774581212, -8.669841519149);  // D₁₀
    glVertex2f(12.082385271265, -7.905213439538);  // F₁₀
    glVertex2f(12.761766844897, -8.064532834253);  // T₁₂
    glVertex2f(13.080737086123, -8.172926897853);  // H₁₀
    glEnd();

    //triangulo 364 - 7
    glBegin(GL_TRIANGLES);
    glColor3ub(45, 54, 44);
    glVertex2f(13.080737086123, -8.172926897853);  // H₁₀
    glVertex2f(12.869835276059, -8.927156133831);  // I₁₀
    glVertex2f(11.916774581212, -8.669841519149);  // D₁₀
    glEnd();

    //triangulo 365 - 8
    glBegin(GL_TRIANGLES);
    glColor3ub(25, 42, 35);
    glVertex2f(13.080737086123, -8.172926897853);  // H₁₀
    glVertex2f(13.598680968036, -9.144259105910);  // L₁₀
    glVertex2f(12.869835276059, -8.927156133831);  // I₁₀
    glEnd();

    //triangulo 366 - 9
    glBegin(GL_TRIANGLES);
    glColor3ub(23, 41, 32);
    glVertex2f(13.080737086123, -8.172926897853);  // H₁₀
    glVertex2f(13.815783940115, -8.772082582346);  // M₁₀
    glVertex2f(13.598680968036, -9.144259105910);  // L₁₀
    glEnd();

    //triangulo 367 - 10
    glBegin(GL_TRIANGLES);
    glColor3ub(19, 38, 30);
    glVertex2f(13.815783940115, -8.772082582346);  // M₁₀
    glVertex2f(14.217819780992, -9.510050780367);  // N₁₀
    glVertex2f(13.598680968036, -9.144259105910);  // L₁₀
    glEnd();

    //triangulo 368 - 11
    glBegin(GL_TRIANGLES);
    glColor3ub(14, 33, 25);
    glVertex2f(13.598680968036, -9.144259105910);  // L₁₀
    glVertex2f(14.217819780992, -9.510050780367);  // N₁₀
    glVertex2f(13.248141556562, -9.835662287464);  // K₁₀
    glEnd();

    //triangulo 369 - 12
    glBegin(GL_TRIANGLES);
    glColor3ub(9, 29, 21);
    glVertex2f(13.248141556562, -9.835662287464);  // K₁₀
    glVertex2f(14.217819780992, -9.510050780367);  // N₁₀
    glVertex2f(13.815783940115, -10.012670994224);  // P₁₀
    glEnd();

    //triangulo 370 - 13
    glBegin(GL_TRIANGLES);
    glColor3ub(10, 28, 21);
    glVertex2f(13.248141556562, -9.835662287464);  // K₁₀
    glVertex2f(13.815783940115, -10.012670994224);  // P₁₀
    glVertex2f(12.866748565170, -10.236414911582);  // O₁₀
    glEnd();

    //triangulo 371 - 14
    glBegin(GL_TRIANGLES);
    glColor3ub(16, 32, 23);
    glVertex2f(12.187511649526, -9.818829054886);  // J₁₀
    glVertex2f(13.248141556562, -9.835662287464);  // K₁₀
    glVertex2f(12.866748565170, -10.236414911582);  // O₁₀
    glEnd();

    //triangulo 372 - 15
    glBegin(GL_TRIANGLES);
    glColor3ub(23, 38, 30);
    glVertex2f(12.187511649526, -9.818829054886);  // J₁₀
    glVertex2f(12.869835276059, -8.927156133831);  // I₁₀
    glVertex2f(13.248141556562, -9.835662287464);  // K₁₀
    glEnd();

    //triangulo 373 - 16
    glBegin(GL_TRIANGLES);
    glColor3ub(35, 47, 37);
    glVertex2f(11.916774581212, -8.669841519149);  // D₁₀
    glVertex2f(12.869835276059, -8.927156133831);  // I₁₀
    glVertex2f(12.187511649526, -9.818829054886);  // J₁₀
    glEnd();

    //triangulo 374 - 17
    glBegin(GL_TRIANGLES);
    glColor3ub(51, 53, 39);
    glVertex2f(11.916774581212, -8.669841519149);  // D₁₀
    glVertex2f(12.187511649526, -9.818829054886);  // J₁₀
    glVertex2f(11.356517140391, -9.455611389979);  // C₁₀
    glEnd();

    //triangulo 375 - 18
    glBegin(GL_TRIANGLES);
    glColor3ub(22, 36, 22);
    glVertex2f(12.187511649526, -9.818829054886);  // J₁₀
    glVertex2f(12.866748565170, -10.236414911582);  // O₁₀
    glVertex2f(11.092244762645, -9.991203407877);  // A₁₀
    glEnd();

    //triangulo 376 - 19
    glBegin(GL_TRIANGLES);
    glColor3ub(37, 45, 30);
    glVertex2f(11.092244762645, -9.991203407877);  // A₁₀
    glVertex2f(11.356517140391, -9.455611389979);  // C₁₀
    glVertex2f(12.187511649526, -9.818829054886);  // J₁₀
    glEnd();

    //triangulo 377 - 20
    glBegin(GL_POLYGON);
    glColor3ub(88, 73, 40);
    glVertex2f(11.357868149083, -8.702299484178);  // E₁₀
    glVertex2f(11.916774581212, -8.669841519149);  // D₁₀
    glVertex2f(11.356517140391, -9.455611389979);  // C₁₀
    glVertex2f(10.926634072591, -9.476753179199);  // B₁₀
    glEnd();

    //triangulo 378 - 21
    glBegin(GL_TRIANGLES);
    glColor3ub(49, 52, 30);
    glVertex2f(11.092244762645, -9.991203407877);  // A₁₀
    glVertex2f(10.926634072591, -9.476753179199);  // B₁₀
    glVertex2f(11.356517140391, -9.455611389979);  // C₁₀
    glEnd();

    //triangulo 379 - 22
    glBegin(GL_TRIANGLES);
    glColor3ub(57, 56, 27);
    glVertex2f(10.334663946441, -9.684674449692);  // R₉
    glVertex2f(10.926634072591, -9.476753179199);  // B₁₀
    glVertex2f(11.092244762645, -9.991203407877);  // A₁₀
    glEnd();

    //triangulo 379 - 23
    glBegin(GL_TRIANGLES);
    glColor3ub(19, 37, 30);
    glVertex2f(12.869835276059, -8.927156133831);  // I₁₀
    glVertex2f(13.598680968036, -9.144259105910);  // L₁₀
    glVertex2f(13.248141556562, -9.835662287464);  // K₁₀
    glEnd();

    glPopMatrix();
    glPopMatrix();
    glPopMatrix();


    glPushMatrix();  // Guardar la matriz actual
    glTranslatef(3.4f, 3.6f, 0.0f);  // mover al destino

    glPushMatrix();
    // Trasladar al centro, voltear, y devolver
    glTranslatef(14.8f, 0.0f, 0.0f); 
    glScalef(1.0, 1.3, 1.0);      // voltear en X
    glTranslatef(-14.8f, 0.0f, 0.0f); 
    // Roca 1
        //triangulo 358 - 1
    glBegin(GL_POLYGON);
    glColor3ub(91, 74, 35);
    glVertex2f(10.334663946441, -9.684674449692);  // R₉
    glVertex2f(10.507321899923, -9.212480801453);  // S₉
    glVertex2f(10.588365429077, -9.057441006509);  // Q₁₀
    glVertex2f(10.926634072591, -9.476753179199);  // B₁₀
    glEnd();

    //triangulo 359 - 2
    glBegin(GL_TRIANGLES);
    glColor3ub(99, 79, 39);
    glVertex2f(10.588365429077, -9.057441006509);  // Q₁₀
    glVertex2f(11.357868149083, -8.702299484178);  // E₁₀
    glVertex2f(10.926634072591, -9.476753179199);  // B₁₀
    glEnd();

    //triangulo 360 - 3
    glBegin(GL_POLYGON);
    glColor3ub(136, 101, 38);
    glVertex2f(10.588365429077, -9.057441006509);  // Q₁₀
    glVertex2f(10.799561000230, -8.815531895257);  // T₁₁
    glVertex2f(11.2, -8.4);             // G₁₀
    glVertex2f(11.357868149083, -8.702299484178);  // E₁₀
    glEnd();

    //triangulo 361 - 4
    glBegin(GL_TRIANGLES);
    glColor3ub(138, 100, 42);
    glVertex2f(11.2, -8.4);             // G₁₀
    glVertex2f(12.082385271265, -7.905213439538);  // F₁₀
    glVertex2f(11.357868149083, -8.702299484178);  // E₁₀
    glEnd();

    //triangulo 362 - 5
    glBegin(GL_TRIANGLES);
    glColor3ub(131, 95, 43);
    glVertex2f(11.357868149083, -8.702299484178);  // E₁₀
    glVertex2f(12.082385271265, -7.905213439538);  // F₁₀
    glVertex2f(11.916774581212, -8.669841519149);  // D₁₀
    glEnd();

    //triangulo 363 - 6
    glBegin(GL_POLYGON);
    glColor3ub(77, 68, 43);
    glVertex2f(11.916774581212, -8.669841519149);  // D₁₀
    glVertex2f(12.082385271265, -7.905213439538);  // F₁₀
    glVertex2f(12.761766844897, -8.064532834253);  // T₁₂
    glVertex2f(13.080737086123, -8.172926897853);  // H₁₀
    glEnd();

    //triangulo 364 - 7
    glBegin(GL_TRIANGLES);
    glColor3ub(45, 54, 44);
    glVertex2f(13.080737086123, -8.172926897853);  // H₁₀
    glVertex2f(12.869835276059, -8.927156133831);  // I₁₀
    glVertex2f(11.916774581212, -8.669841519149);  // D₁₀
    glEnd();

    //triangulo 365 - 8
    glBegin(GL_TRIANGLES);
    glColor3ub(25, 42, 35);
    glVertex2f(13.080737086123, -8.172926897853);  // H₁₀
    glVertex2f(13.598680968036, -9.144259105910);  // L₁₀
    glVertex2f(12.869835276059, -8.927156133831);  // I₁₀
    glEnd();

    //triangulo 366 - 9
    glBegin(GL_TRIANGLES);
    glColor3ub(23, 41, 32);
    glVertex2f(13.080737086123, -8.172926897853);  // H₁₀
    glVertex2f(13.815783940115, -8.772082582346);  // M₁₀
    glVertex2f(13.598680968036, -9.144259105910);  // L₁₀
    glEnd();

    //triangulo 367 - 10
    glBegin(GL_TRIANGLES);
    glColor3ub(19, 38, 30);
    glVertex2f(13.815783940115, -8.772082582346);  // M₁₀
    glVertex2f(14.217819780992, -9.510050780367);  // N₁₀
    glVertex2f(13.598680968036, -9.144259105910);  // L₁₀
    glEnd();

    //triangulo 368 - 11
    glBegin(GL_TRIANGLES);
    glColor3ub(14, 33, 25);
    glVertex2f(13.598680968036, -9.144259105910);  // L₁₀
    glVertex2f(14.217819780992, -9.510050780367);  // N₁₀
    glVertex2f(13.248141556562, -9.835662287464);  // K₁₀
    glEnd();

    //triangulo 369 - 12
    glBegin(GL_TRIANGLES);
    glColor3ub(9, 29, 21);
    glVertex2f(13.248141556562, -9.835662287464);  // K₁₀
    glVertex2f(14.217819780992, -9.510050780367);  // N₁₀
    glVertex2f(13.815783940115, -10.012670994224);  // P₁₀
    glEnd();

    //triangulo 370 - 13
    glBegin(GL_TRIANGLES);
    glColor3ub(10, 28, 21);
    glVertex2f(13.248141556562, -9.835662287464);  // K₁₀
    glVertex2f(13.815783940115, -10.012670994224);  // P₁₀
    glVertex2f(12.866748565170, -10.236414911582);  // O₁₀
    glEnd();

    //triangulo 371 - 14
    glBegin(GL_TRIANGLES);
    glColor3ub(16, 32, 23);
    glVertex2f(12.187511649526, -9.818829054886);  // J₁₀
    glVertex2f(13.248141556562, -9.835662287464);  // K₁₀
    glVertex2f(12.866748565170, -10.236414911582);  // O₁₀
    glEnd();

    //triangulo 372 - 15
    glBegin(GL_TRIANGLES);
    glColor3ub(23, 38, 30);
    glVertex2f(12.187511649526, -9.818829054886);  // J₁₀
    glVertex2f(12.869835276059, -8.927156133831);  // I₁₀
    glVertex2f(13.248141556562, -9.835662287464);  // K₁₀
    glEnd();

    //triangulo 373 - 16
    glBegin(GL_TRIANGLES);
    glColor3ub(35, 47, 37);
    glVertex2f(11.916774581212, -8.669841519149);  // D₁₀
    glVertex2f(12.869835276059, -8.927156133831);  // I₁₀
    glVertex2f(12.187511649526, -9.818829054886);  // J₁₀
    glEnd();

    //triangulo 374 - 17
    glBegin(GL_TRIANGLES);
    glColor3ub(51, 53, 39);
    glVertex2f(11.916774581212, -8.669841519149);  // D₁₀
    glVertex2f(12.187511649526, -9.818829054886);  // J₁₀
    glVertex2f(11.356517140391, -9.455611389979);  // C₁₀
    glEnd();

    //triangulo 375 - 18
    glBegin(GL_TRIANGLES);
    glColor3ub(22, 36, 22);
    glVertex2f(12.187511649526, -9.818829054886);  // J₁₀
    glVertex2f(12.866748565170, -10.236414911582);  // O₁₀
    glVertex2f(11.092244762645, -9.991203407877);  // A₁₀
    glEnd();

    //triangulo 376 - 19
    glBegin(GL_TRIANGLES);
    glColor3ub(37, 45, 30);
    glVertex2f(11.092244762645, -9.991203407877);  // A₁₀
    glVertex2f(11.356517140391, -9.455611389979);  // C₁₀
    glVertex2f(12.187511649526, -9.818829054886);  // J₁₀
    glEnd();

    //triangulo 377 - 20
    glBegin(GL_POLYGON);
    glColor3ub(88, 73, 40);
    glVertex2f(11.357868149083, -8.702299484178);  // E₁₀
    glVertex2f(11.916774581212, -8.669841519149);  // D₁₀
    glVertex2f(11.356517140391, -9.455611389979);  // C₁₀
    glVertex2f(10.926634072591, -9.476753179199);  // B₁₀
    glEnd();

    //triangulo 378 - 21
    glBegin(GL_TRIANGLES);
    glColor3ub(49, 52, 30);
    glVertex2f(11.092244762645, -9.991203407877);  // A₁₀
    glVertex2f(10.926634072591, -9.476753179199);  // B₁₀
    glVertex2f(11.356517140391, -9.455611389979);  // C₁₀
    glEnd();

    //triangulo 379 - 22
    glBegin(GL_TRIANGLES);
    glColor3ub(57, 56, 27);
    glVertex2f(10.334663946441, -9.684674449692);  // R₉
    glVertex2f(10.926634072591, -9.476753179199);  // B₁₀
    glVertex2f(11.092244762645, -9.991203407877);  // A₁₀
    glEnd();

    //triangulo 379 - 23
    glBegin(GL_TRIANGLES);
    glColor3ub(19, 37, 30);
    glVertex2f(12.869835276059, -8.927156133831);  // I₁₀
    glVertex2f(13.598680968036, -9.144259105910);  // L₁₀
    glVertex2f(13.248141556562, -9.835662287464);  // K₁₀
    glEnd();

    glPopMatrix();
    glPopMatrix();

    //Roca 2 
    
    //triangulo 384 - 1
    glBegin(GL_TRIANGLES);
    glColor3ub(44, 55, 35);
    glVertex2f(14.068269600788, -10.006362721322);  // R₁₀
    glVertex2f(14.369502668818, -9.580429449272);  // S₁₀
    glVertex2f(14.432689938055, -9.725455378012);  // T₁₀
    glEnd();

    //triangulo 385 - 2
    glBegin(GL_POLYGON);
    glColor3ub(73, 70, 40);
    glVertex2f(14.369502668818, -9.580429449272);  // S₁₀
    glVertex2f(14.713597281364, -9.300298317866);  // V₁₀
    glVertex2f(15.096997844530, -9.562225435278);  // U₁₀
    glVertex2f(14.432689938055, -9.725455378012);  // T₁₀
    glEnd();

    //triangulo 386 - 3
    glBegin(GL_TRIANGLES);
    glColor3ub(81, 73, 43);
    glVertex2f(14.713597281364, -9.300298317866);  // V₁₀
    glVertex2f(15.150142477048, -9.281318091969);  // Z₁₀
    glVertex2f(15.096997844530, -9.562225435278);  // U₁₀
    glEnd();

    //triangulo 387 - 4
    glBegin(GL_TRIANGLES);
    glColor3ub(39, 53, 43);
    glVertex2f(15.096997844530, -9.562225435278);  // U₁₀
    glVertex2f(15.150142477048, -9.281318091969);  // Z₁₀
    glVertex2f(15.647424395610, -9.486304531681);  // W₁₀
    glEnd();

    //triangulo 388 - 5
    glBegin(GL_TRIANGLES);
    glColor3ub(22, 43, 35);
    glVertex2f(15.096997844530, -9.562225435278);  // U₁₀
    glVertex2f(15.647424395610, -9.486304531681);  // W₁₀
    glVertex2f(15.662608576331, -10.040527127941);  // A₁₁
    glEnd();

    //triangulo 389 - 6
    glBegin(GL_TRIANGLES);
    glColor3ub(16, 39, 34);
    glVertex2f(15.647424395610, -9.486304531681);  // W₁₀
    glVertex2f(15.977680326258, -10.032930375817);  // C₁₁
    glVertex2f(15.662608576331, -10.040527127941);  // A₁₁
    glEnd();

    //triangulo 390 - 7
    glBegin(GL_TRIANGLES);
    glColor3ub(10,33, 28);
    glVertex2f(15.662608576331, -10.040527127941);  // A₁₁
    glVertex2f(15.977680326258, -10.032930375817);  // C₁₁
    glVertex2f(15.480398076967, -10.401151420028);  // D₁₁
    glEnd();

    //triangulo 391 - 8
    glBegin(GL_TRIANGLES);
    glColor3ub(17, 40, 32);
    glVertex2f(14.861643043379, -10.0595073538407);  // B₁₁
    glVertex2f(15.662608576331, -10.040527127941);  // A₁₁
    glVertex2f(15.480398076967, -10.401151420028);  // D₁₁
    glEnd();

    //triangulo 392 - 9
    glBegin(GL_TRIANGLES);
    glColor3ub(28, 44, 36);
    glVertex2f(15.096997844530, -9.562225435278);  // U₁₀
    glVertex2f(15.662608576331, -10.040527127941);  // A₁₁
    glVertex2f(14.861643043379, -10.0595073538407);  // B₁₁
    glEnd();

    //triangulo 393 - 10
    glBegin(GL_TRIANGLES);
    glColor3ub(12, 35, 28);
    glVertex2f(14.561755474170, -10.264493793553);  // E₁₁
    glVertex2f(14.861643043379, -10.0595073538407);  // B₁₁
    glVertex2f(15.480398076967, -10.401151420028);  // D₁₁
    glEnd();

    //triangulo 394 - 11
    glBegin(GL_TRIANGLES);
    glColor3ub(48, 55, 39);
    glVertex2f(14.432689938055, -9.725455378012);  // T₁₀
    glVertex2f(15.096997844530, -9.562225435278);  // U₁₀
    glVertex2f(14.861643043379, -10.0595073538407);  // B₁₁
    glEnd();

    //triangulo 395 - 12
    glBegin(GL_POLYGON);
    glColor3ub(29, 45, 32);
    glVertex2f(14.432689938055, -9.725455378012);  // T₁₀
    glVertex2f(14.861643043379, -10.0595073538407);  // B₁₁
    glVertex2f(14.561755474170, -10.264493793553);  // E₁₁
    glVertex2f(14.068269600788, -10.006362721322);  // R₁₀
    glEnd();

    // Roca 1
        //triangulo 358 - 1
    glBegin(GL_POLYGON);
    glColor3ub(91, 74, 35);
    glVertex2f(10.334663946441, -9.684674449692);  // R₉
    glVertex2f(10.507321899923, -9.212480801453);  // S₉
    glVertex2f(10.588365429077, -9.057441006509);  // Q₁₀
    glVertex2f(10.926634072591, -9.476753179199);  // B₁₀
    glEnd();

    //triangulo 359 - 2
    glBegin(GL_TRIANGLES);
    glColor3ub(99, 79, 39);
    glVertex2f(10.588365429077, -9.057441006509);  // Q₁₀
    glVertex2f(11.357868149083, -8.702299484178);  // E₁₀
    glVertex2f(10.926634072591, -9.476753179199);  // B₁₀
    glEnd();

    //triangulo 360 - 3
    glBegin(GL_POLYGON);
    glColor3ub(136, 101, 38);
    glVertex2f(10.588365429077, -9.057441006509);  // Q₁₀
    glVertex2f(10.799561000230, -8.815531895257);  // T₁₁
    glVertex2f(11.2, -8.4);             // G₁₀
    glVertex2f(11.357868149083, -8.702299484178);  // E₁₀
    glEnd();

    //triangulo 361 - 4
    glBegin(GL_TRIANGLES);
    glColor3ub(138, 100, 42);
    glVertex2f(11.2, -8.4);             // G₁₀
    glVertex2f(12.082385271265, -7.905213439538);  // F₁₀
    glVertex2f(11.357868149083, -8.702299484178);  // E₁₀
    glEnd();

    //triangulo 362 - 5
    glBegin(GL_TRIANGLES);
    glColor3ub(131, 95, 43);
    glVertex2f(11.357868149083, -8.702299484178);  // E₁₀
    glVertex2f(12.082385271265, -7.905213439538);  // F₁₀
    glVertex2f(11.916774581212, -8.669841519149);  // D₁₀
    glEnd();

    //triangulo 363 - 6
    glBegin(GL_POLYGON);
    glColor3ub(77, 68, 43);
    glVertex2f(11.916774581212, -8.669841519149);  // D₁₀
    glVertex2f(12.082385271265, -7.905213439538);  // F₁₀
    glVertex2f(12.761766844897, -8.064532834253);  // T₁₂
    glVertex2f(13.080737086123, -8.172926897853);  // H₁₀
    glEnd();

    //triangulo 364 - 7
    glBegin(GL_TRIANGLES);
    glColor3ub(45, 54, 44);
    glVertex2f(13.080737086123, -8.172926897853);  // H₁₀
    glVertex2f(12.869835276059, -8.927156133831);  // I₁₀
    glVertex2f(11.916774581212, -8.669841519149);  // D₁₀
    glEnd();

    //triangulo 365 - 8
    glBegin(GL_TRIANGLES);
    glColor3ub(25, 42, 35);
    glVertex2f(13.080737086123, -8.172926897853);  // H₁₀
    glVertex2f(13.598680968036, -9.144259105910);  // L₁₀
    glVertex2f(12.869835276059, -8.927156133831);  // I₁₀
    glEnd();

    //triangulo 366 - 9
    glBegin(GL_TRIANGLES);
    glColor3ub(23, 41, 32);
    glVertex2f(13.080737086123, -8.172926897853);  // H₁₀
    glVertex2f(13.815783940115, -8.772082582346);  // M₁₀
    glVertex2f(13.598680968036, -9.144259105910);  // L₁₀
    glEnd();

    //triangulo 367 - 10
    glBegin(GL_TRIANGLES);
    glColor3ub(19, 38, 30);
    glVertex2f(13.815783940115, -8.772082582346);  // M₁₀
    glVertex2f(14.217819780992, -9.510050780367);  // N₁₀
    glVertex2f(13.598680968036, -9.144259105910);  // L₁₀
    glEnd();

    //triangulo 368 - 11
    glBegin(GL_TRIANGLES);
    glColor3ub(14, 33, 25);
    glVertex2f(13.598680968036, -9.144259105910);  // L₁₀
    glVertex2f(14.217819780992, -9.510050780367);  // N₁₀
    glVertex2f(13.248141556562, -9.835662287464);  // K₁₀
    glEnd();

    //triangulo 369 - 12
    glBegin(GL_TRIANGLES);
    glColor3ub(9, 29, 21);
    glVertex2f(13.248141556562, -9.835662287464);  // K₁₀
    glVertex2f(14.217819780992, -9.510050780367);  // N₁₀
    glVertex2f(13.815783940115, -10.012670994224);  // P₁₀
    glEnd();

    //triangulo 370 - 13
    glBegin(GL_TRIANGLES);
    glColor3ub(10, 28, 21);
    glVertex2f(13.248141556562, -9.835662287464);  // K₁₀
    glVertex2f(13.815783940115, -10.012670994224);  // P₁₀
    glVertex2f(12.866748565170, -10.236414911582);  // O₁₀
    glEnd();

    //triangulo 371 - 14
    glBegin(GL_TRIANGLES);
    glColor3ub(16, 32, 23);
    glVertex2f(12.187511649526, -9.818829054886);  // J₁₀
    glVertex2f(13.248141556562, -9.835662287464);  // K₁₀
    glVertex2f(12.866748565170, -10.236414911582);  // O₁₀
    glEnd();

    //triangulo 372 - 15
    glBegin(GL_TRIANGLES);
    glColor3ub(23, 38, 30);
    glVertex2f(12.187511649526, -9.818829054886);  // J₁₀
    glVertex2f(12.869835276059, -8.927156133831);  // I₁₀
    glVertex2f(13.248141556562, -9.835662287464);  // K₁₀
    glEnd();

    //triangulo 373 - 16
    glBegin(GL_TRIANGLES);
    glColor3ub(35, 47, 37);
    glVertex2f(11.916774581212, -8.669841519149);  // D₁₀
    glVertex2f(12.869835276059, -8.927156133831);  // I₁₀
    glVertex2f(12.187511649526, -9.818829054886);  // J₁₀
    glEnd();

    //triangulo 374 - 17
    glBegin(GL_TRIANGLES);
    glColor3ub(51, 53, 39);
    glVertex2f(11.916774581212, -8.669841519149);  // D₁₀
    glVertex2f(12.187511649526, -9.818829054886);  // J₁₀
    glVertex2f(11.356517140391, -9.455611389979);  // C₁₀
    glEnd();

    //triangulo 375 - 18
    glBegin(GL_TRIANGLES);
    glColor3ub(22, 36, 22);
    glVertex2f(12.187511649526, -9.818829054886);  // J₁₀
    glVertex2f(12.866748565170, -10.236414911582);  // O₁₀
    glVertex2f(11.092244762645, -9.991203407877);  // A₁₀
    glEnd();

    //triangulo 376 - 19
    glBegin(GL_TRIANGLES);
    glColor3ub(37, 45, 30);
    glVertex2f(11.092244762645, -9.991203407877);  // A₁₀
    glVertex2f(11.356517140391, -9.455611389979);  // C₁₀
    glVertex2f(12.187511649526, -9.818829054886);  // J₁₀
    glEnd();

    //triangulo 377 - 20
    glBegin(GL_POLYGON);
    glColor3ub(88, 73, 40);
    glVertex2f(11.357868149083, -8.702299484178);  // E₁₀
    glVertex2f(11.916774581212, -8.669841519149);  // D₁₀
    glVertex2f(11.356517140391, -9.455611389979);  // C₁₀
    glVertex2f(10.926634072591, -9.476753179199);  // B₁₀
    glEnd();

    //triangulo 378 - 21
    glBegin(GL_TRIANGLES);
        glColor3ub(49, 52, 30);
        glVertex2f(11.092244762645, -9.991203407877);  // A₁₀
        glVertex2f(10.926634072591, -9.476753179199);  // B₁₀
        glVertex2f(11.356517140391, -9.455611389979);  // C₁₀
    glEnd();

    //triangulo 379 - 22
    glBegin(GL_TRIANGLES);
        glColor3ub(57, 56, 27);
        glVertex2f(10.334663946441, -9.684674449692);  // R₉
        glVertex2f(10.926634072591, -9.476753179199);  // B₁₀
        glVertex2f(11.092244762645, -9.991203407877);  // A₁₀
    glEnd();

    //triangulo 379 - 23
    glBegin(GL_TRIANGLES);
        glColor3ub(19, 37, 30);
        glVertex2f(12.869835276059, -8.927156133831);  // I₁₀
        glVertex2f(13.598680968036, -9.144259105910);  // L₁₀
        glVertex2f(13.248141556562, -9.835662287464);  // K₁₀
    glEnd();

    glFlush();

}

void Cuadrado(){
    // Dibuja un cuadrado simple en 2D

    glBegin(GL_TRIANGLES);
    glColor3ub(255, 233, 147);
    glVertex2f(9.9518225260134, -8.9291584484637); // I
    glVertex2f(10.356517140391, -9.455611389979); // E
    glVertex2f(9.8240120694877, -9.455611389979); // H
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(255, 233, 147);
    glVertex2f(10.356517140391, -9.455611389979); // E
    glVertex2f(10.755202538461, -8.9291584484637); // G
    glVertex2f(10.8869226906358, -9.455611389979); // F
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 193, 5);
    glVertex2f(10.8374138355564, -9.257736714333); // R
    glVertex2f(11.3333926989346, -9.148262088222); // D
    glVertex2f(11.0521360694943, -9.455611389979); // Q
    glVertex2f(10.8869226906358, -9.455611389979); // F
    glEnd();

    glBegin(GL_TRIANGLES); 
    glColor3ub(255, 233, 147);
    glVertex2f(11.0521360694943, -9.455611389979); // Q
    glVertex2f(11.3333926989346, -9.148262088222); // D
    glVertex2f(11.344496834759, -9.455611389979); // P
    glEnd();

    glBegin(GL_POLYGON);
    glColor3ub(255, 193, 5);
    glVertex2f(10.05931663941, -8.5805257774854); // S
    glVertex2f(10.3595987444527, -8.783089352915); // C
    glVertex2f(10.6598668939199, -8.5767292514895); // T
    glVertex2f(10.755202538461, -8.9291584484637); // G
    glVertex2f(10.356517140391, -9.455611389979); // E
    glVertex2f(9.9518225260134, -8.9291584484637); // I
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(255, 233, 147);
    glVertex2f(10.0370294970306, -8.1075197993696); // J
    glVertex2f(10.3595987444527, -8.783089352915); // C
    glVertex2f(9.8492611779853, -8.4388270712044); // N
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(255, 193, 5);
    glVertex2f(10.0370294970306, -8.1075197993696); // J
    glVertex2f(10.6639093552283, -8.1075197993696); // K
    glVertex2f(10.3595987444527, -8.783089352915); // C
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(255, 233, 147);
    glVertex2f(10.6639093552283, -8.1075197993696); // K
    glVertex2f(10.8605240544796, -8.4388270712044); // M
    glVertex2f(10.3595987444527, -8.783089352915); // C
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(255, 193, 5);
    glVertex2f(10.8951854194178, -7.7656495106687); // L
    glVertex2f(10.8605240544796, -8.4388270712044); // M
    glVertex2f(10.6639093552283, -8.1075197993696); // K
    glEnd();

    glBegin(GL_TRIANGLES);
    glColor3ub(255, 193, 5);
    glVertex2f(9.814455032194, -7.7656495106687); // O
    glVertex2f(10.0370294970306, -8.1075197993696); // J
    glVertex2f(9.8492611779853, -8.4388270712044); // N
    glEnd();
    
}

void Roca(){

    glPushMatrix();
    glBegin(GL_TRIANGLES);
        glColor3ub(23.0f, 23.0f, 23.0f);     // Verde brillante
        glVertex2f(0.3596431533187, 0.4805569140954); // A
        glVertex2f(0.7229898857531, 0.2037948573619); // G
        glVertex2f(0.5692954343327, 0); // H
    glEnd();

    glBegin(GL_TRIANGLES);
        glColor3ub(20.0, 20.0, 20.0);     // Azul brillante
        glVertex2f(0.5692954343327, 0); // H
        glVertex2f(0.7229898857531, 0.2037948573619); // G
        glVertex2f(1.3347672923653, 0); // I
    glEnd();

    glBegin(GL_TRIANGLES);
        glColor3ub(33.0, 33.0, 33.0);     // Amarillo brillante
        glVertex2f(0.7229898857531, 0.2037948573619); // G
        glVertex2f(1.1915705303274, 0.3481176958908); // F
        glVertex2f(1.3347672923653, 0); // I
    glEnd();

    glBegin(GL_POLYGON);
        glColor3ub(57.0, 57.0, 57.0);     // Rojo brillante
        glVertex2f(0.3596431533187, 0.4805569140954); // A
        glVertex2f(0.8495510553555, 1.1747935856191); // P
        glVertex2f(1.1915705303274, 0.3481176958908); // F
        glVertex2f(0.7229898857531, 0.2037948573619); // G
    glEnd();

    glBegin(GL_POLYGON);
        glColor3ub(33.0, 33.0, 33.0);     // Magenta brillante
        glVertex2f(0.8495510553555, 1.1747935856191); // P
        glVertex2f(0.8830339584575, 2.0279889675609); // O
        glVertex2f(1.6778494801499, 0.7287867414991); // L
        glVertex2f(1.3347672923653, 0); // I
    glEnd();

    glBegin(GL_POLYGON);
        glColor3ub(57.0, 57.0, 57.0);
        glVertex2f(1.3347672923653, 0); // I
        glVertex2f(1.6778494801499, 0.7287867414991); // L
        glVertex2f(1.9585645866431, 0.6145397803412); // K
        glVertex2f(2.0697443930452, 0.3403142664689); // J
    glEnd();

    glBegin(GL_TRIANGLES);
        glColor3ub(22.0, 24.5, 23.0);
        glVertex2f(1.3347672923653, 0); // I
        glVertex2f(2.0697443930452, 0.3403142664689); // J
        glVertex2f(2.4, 0); // N
    glEnd();

    glBegin(GL_POLYGON);
        glColor3ub(87.0, 87.0, 87.0);
        glVertex2f(1.9585645866431, 0.6145397803412); // K
        glVertex2f(2.5419648081925, 0.7263031275459); // M
        glVertex2f(2.4, 0); // N
        glVertex2f(2.0697443930452, 0.3403142664689); // J
    glEnd();

    glBegin(GL_POLYGON);
        glColor3ub(147.0, 147.0, 147.0);
        glVertex2f(0.8830339584575, 2.0279889675609); // O
        glVertex2f(2.5419648081925, 0.7263031275459); // M
        glVertex2f(1.9585645866431, 0.6145397803412); // K
        glVertex2f(1.6778494801499, 0.7287867414991); // L
    glEnd();
    glPopMatrix();


}

void dibujarFigura()
{
    glScalef(0.3f, 0.3f, 1.0f);      // Escala apropiada para ser visible
    glColor3f(1.0f, 1.0f, 0.0f);     // Amarillo brillante
    Roca();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    arbolito();

    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(tx + mov, ty, 0); // tx para salto, mov para movimiento horizontal normal
    Cuadrado();
    glPopMatrix();

        // Dibujar las N figuras
    for (int i = 0; i < N; i++) {
        glPushMatrix();
        glTranslatef(posX[i], -9.5f, 0.0f);    // posición horizontal y misma altura que el cuadrado
        float escala = 1.0f + i * 0.5f;       // tamaños diferentes
        glScalef(escala, escala, 1.0f);
        dibujarFigura();
        glPopMatrix();
    }

    glFlush();
}

void manejarTeclasPresionadas()
{
    if (teclaApretada['a'] && !saltando)
    {
        saltando = 1;
        velocidadSalto = 2; //Altura del salto
        direccionSalto = -0.15; // Dirección y distancia hacia la izquierda
    }
    if (teclaApretada['b'] && !saltando)
    {
        saltando = 1;
        velocidadSalto = 2; // Altura del salto
        direccionSalto = 0.15; // Dirección y distancia hacia la derecha
    }
}

void trasladarCuadrado(int)
{
    // Calcular delta de tiempo
    int ahora = glutGet(GLUT_ELAPSED_TIME);
    if (ultimoMs == 0) ultimoMs = ahora;
    float dt = (ahora - ultimoMs) / 1000.0f;
    ultimoMs = ahora;

    // ------------------------
    // 1. Movimiento de figuras
    // ------------------------
    for (int i = 0; i < N; i++) {
        posX[i] += velX[i] * dt;

        // Rebote en bordes
        if (posX[i] > 16.0f || posX[i] < 0.5f) {
            velX[i] = -velX[i];
            if (posX[i] > 16.0f) posX[i] = 16.0f;
            if (posX[i] < 0.5f) posX[i] = 0.5f;
        }
    }

    // ------------------------
    // 2. Movimiento del cuadrado (salto)
    // ------------------------
    if (saltando)
    {
        ty += velocidadSalto;          // Movimiento vertical
        tx += direccionSalto * 2;      // Movimiento horizontal durante salto
        velocidadSalto -= 0.15;        // Gravedad

        if (ty <= 0) {
            ty = 0;
            saltando = 0;
        }
    }

    // Manejar teclas de salto
    if (teclaEspecialApretada[GLUT_KEY_UP] && !saltando)
    {
        saltando = 1;
        velocidadSalto = 1;     // Altura del salto
        direccionSalto = 0;     // Salto vertical sin dirección específica
    }
    // Saltos direccionales originales
    else if (teclaApretada['a'] && !saltando)
    {
        saltando = 1;
        velocidadSalto = 1;     // Altura del salto
        direccionSalto = -0.10; // Izquierda
    }
    else if (teclaApretada['b'] && !saltando)
    {
        saltando = 1;
        velocidadSalto = 1;     // Altura del salto
        direccionSalto = 0.10;  // Derecha
    }

    // Redibujar escena
    glutPostRedisplay();

    // Programar el próximo frame (~60 FPS)
    glutTimerFunc(16, trasladarCuadrado, 0);
}

void movimientoHorizontal(int)
{
    // Calcular delta de tiempo
    int ahora = glutGet(GLUT_ELAPSED_TIME);
    static int ultimoMsHorizontal = 0;
    if (ultimoMsHorizontal == 0) ultimoMsHorizontal = ahora;
    float dt = (ahora - ultimoMsHorizontal) / 1000.0f;
    ultimoMsHorizontal = ahora;

    // Movimiento horizontal continuo usando la variable mov
    if (teclaApretada['d'] || teclaEspecialApretada[GLUT_KEY_RIGHT]) { // Mover a la derecha
        mov += velocidadHorizontal * dt;
    }
    if (teclaApretada['s'] || teclaEspecialApretada[GLUT_KEY_LEFT]) { // Mover a la izquierda
        mov -= velocidadHorizontal * dt;
    }
    
    // Sin limitaciones - movimiento libre en ambas direcciones

    // Programar el próximo frame para movimiento horizontal (~60 FPS)
    glutTimerFunc(16, movimientoHorizontal, 0);
}

void manejarTeclado(unsigned char key, int x, int y)
{
    teclaApretada[key] = 1; // Registrar que la tecla está presionada
}

void manejarTecladoSoltar(unsigned char key, int x, int y)
{
    teclaApretada[key] = 0; // Registrar que la tecla fue soltada
}

void manejarTeclasEspeciales(int key, int x, int y)
{
    teclaEspecialApretada[key] = 1; // Registrar que la tecla especial está presionada
}

void manejarTeclasEspecialesSoltar(int key, int x, int y)
{
    teclaEspecialApretada[key] = 0; // Registrar que la tecla especial fue soltada
}

void init() {
    glClearColor(0.0f, 0.25f, 0.1f, 1.0f);
    glColor3ub(255, 255, 255);
    glPointSize(1.0f);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 16.5, -11.03, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE);
    glutInitWindowSize(1136, 624);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Montañas con OpenGL");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(manejarTeclado);
    glutKeyboardUpFunc(manejarTecladoSoltar);
    glutSpecialFunc(manejarTeclasEspeciales);
    glutSpecialUpFunc(manejarTeclasEspecialesSoltar);
    glutTimerFunc(50, trasladarCuadrado, 1);
    glutTimerFunc(50, movimientoHorizontal, 1); // Inicializar movimiento horizontal



    glutMainLoop();
    return 0;
}