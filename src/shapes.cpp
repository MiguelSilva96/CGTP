#define _USE_MATH_DEFINES
#include <math.h>
#include <sstream>
#include <fstream>
#include "shapes.h"

/**
    Auxiliar function that returns a string that
    describes one vertex with the coordinates given
    as arguments.

    @param x Position on X axis
    @param y Position on Y axis
    @param z Position on Z axis
    @return String that describes the vertex
*/



std::string vertexString(float x, float y, float z) {
    std::stringstream strStream;
    strStream << x << " ";
    strStream << y << " ";
    strStream << z << "\n";
    return strStream.str();
}

std::vector<std::vector<float>> preenche(int i, std::vector<std::vector<int>> indices,
                                    std::vector<std::vector<float>> controlPoints) {
    std::vector<int> linha = indices[i];
    std::vector<std::vector<float>> setDeControlo;
    int tamanho = indices[i].size();
    int t = 0;
    while (t < tamanho) {
        int n = linha[t];
        setDeControlo.push_back(controlPoints[n]);
        t++;
    }
    return setDeControlo;
}

int factorial(int n) {
  int result = 0;
  if (n >= 0) {
    result = 1;
    for (int i = n; i > 1; i--)
    result *= i;
  }
  return result;
}

float coeficienteBi(int i, int n) {
  if ((i >= 0) && (n >= 0)) {
    return 1.0f * factorial(n) / (factorial(i) * factorial(n-i));
  }
  return 0;
}

float polinomialBerns(int i, int n, float u) {
  return coeficienteBi(i, n) * powf(u, i) * powf(1-u, n-i);
}


std::vector<float> calculaPosicao(std::vector<std::vector<float>> pontosDeControlo,
                                  float u, float v) {
    std::vector<float> result;
    float total1 = 0;
    float total2 = 0;
    float total3 = 0;
    int t = 0;
    for(int i = 0; i < 3; i++) {
      for(int j = 0; j < 3; j++) {
        float polinomial1 = polinomialBerns(i, 3, u);
        float polinomial2 = polinomialBerns(j, 3, v);
        std::vector<float> ponto = pontosDeControlo[t];
        total1 += polinomial1 * polinomial2 * ponto[0];
        total2 += polinomial1 * polinomial2 * ponto[1];
        total3 += polinomial1 * polinomial2 * ponto[2];
        t++;
      }
    }
    result.push_back(total1);
    result.push_back(total2);
    result.push_back(total2);
    return result;
}

void constroiTeapot(std::vector<std::vector<int>> indices,
                    std::vector<std::vector<float>> controlPoints,
                    int nrPatches, int tessellation) {

    std::vector<std::vector<float>> vertices(nrPatches*tessellation*tessellation);
    for(int i = 0; i < 1; i++) {
       std::vector<std::vector<float>> pontosDeControlo;
       pontosDeControlo = preenche(i, indices, controlPoints);
       for (int ru = 0; ru <= (tessellation-1); ru++) {
         float u = 1.0 * ru / (tessellation-1);
         for (int rv = 0; rv <= (tessellation-1); rv++) {
           float v = 1.0 * rv / (tessellation-1);
           std::vector<float> f = calculaPosicao(pontosDeControlo, u, v);
           vertices[((i*tessellation*tessellation) + (ru*tessellation) + rv)] = f;
           for (int j = 0; j < vertices[((i*tessellation*tessellation) + (ru*tessellation) + rv)].size(); j++) {
             printf("%f ", vertices[((i*tessellation*tessellation) + (ru*tessellation) + rv)][j]);
           }
           printf("\n");
         }
       }
     }
    /*
    std::vector<int> elementos;
     int n = 0;
     for (int p = 0; p < nrPatches; p++)
        for (int ru = 0; ru < 9; ru++)
            for (int rv = 0; rv < 9; rv++) {
	             elementos[n] = (p*10*10+ru*10+rv);
               n++;
	             elementos[n] = (p*10*10+ru*10+(rv+1));
               n++;
	             elementos[n] = (p*10*10+(ru+1)*10+(rv+1));
               n++;
	             elementos[n] = (p*10*10+(ru+1)*10+(rv+1));
               n++;
	             elementos[n] = (p*10*10+(ru+1)*10+rv);
               n++;
	             elementos[n] = (p*10*10+ru*10+rv);
               n++;
             }
    */
}


std::vector<int> addIndex(std::string line) {
    std::vector<int> colIndices;
    std::string token;
    size_t pos = 0;
    std::string delimiter = ", ";
    while((pos = line.find(delimiter)) != std::string::npos) {
      token = line.substr(0, pos);
      line.erase(0, pos + delimiter.length());
      int indexValue = std::stoi(token);
      colIndices.push_back(indexValue);
    }
    int indexValue = std::stoi(line);
    colIndices.push_back(indexValue);
    return colIndices;
}

std::vector<float> addPoint(std::string line) {
    std::vector<float> colPoints;
    std::string token;
    size_t pos = 0;
    std::string delimiter = ", ";
    while((pos = line.find(delimiter)) != std::string::npos) {
      token = line.substr(0, pos);
      line.erase(0, pos + delimiter.length());
      float pointValue = std::stof(token);
      colPoints.push_back(pointValue);
    }
    float pointValue = std::stof(line);
    colPoints.push_back(pointValue);
    return colPoints;
}

std::vector<std::string> teatopOrComet(char *input, int tessellation) {
  std::vector<std::string> v;
  std::ifstream readingFile(input);
  int nrPatches = 0;
  int nrControlPoints = 0;
  int i = 0;
  std::vector<int> colIndices;
  std::vector<float> colPoints;
  std::vector<std::vector<int>> indices;
  std::vector<std::vector<float>> controlPoints;
  std::string line;
  while(getline(readingFile, line)) {
    if (i == 0) {
      nrPatches = std::stoi(line);
    }
    else if (i > 0 && i <= nrPatches) {
      colIndices =  addIndex(line);
      indices.push_back(colIndices);
    }
    else if (i == nrPatches + 1) {
      nrControlPoints = std::stoi(line);
    }
    else {
      colPoints = addPoint(line);
      controlPoints.push_back(colPoints);
    }
    i++;
  }
  constroiTeapot(indices,controlPoints, nrPatches, tessellation);
  return v;
}


std::vector<std::string> plane(float x, float y, float z, int div) {
    std::vector<std::string> v;
    for(int i = 0; i < div; i++) {
        for(int j = 0; j < div; j++) {
            v.push_back(vertexString(x/2-x/div*(i+1),
                        y, -z/2+z/div*j));
            v.push_back(vertexString(x/2-x/div*i,
                        y, -z/2+z/div*(j+1)));
            v.push_back(vertexString(x/2-x/div*i,
                        y, -z/2+z/div*j));

            v.push_back(vertexString(x/2-x/div*(i+1),
                        y, -z/2+z/div*j));
            v.push_back(vertexString(x/2-x/div*(i+1),
                        y, -z/2+z/div*(j+1)));
            v.push_back(vertexString(x/2-x/div*i,
                        y, -z/2+z/div*(j+1)));
        }
    }
    return v;
}


std::vector<std::string> box(float x, float y, float z, int div) {
    std::vector<std::string> v;
    //top
    v = plane(x, y/2, z, div);
    for(int i = 0; i < div; i++) {
        for(int j = 0; j < div; j++) {
            //base
            v.push_back(vertexString(x/2-x/div*(i+1), -y/2, -z/2+z/div*j));
            v.push_back(vertexString(x/2-x/div*i, -y/2, -z/2+z/div*j));
            v.push_back(vertexString(x/2-x/div*i, -y/2, -z/2+z/div*(j+1)));

            v.push_back(vertexString(x/2-x/div*(i+1), -y/2, -z/2+z/div*j));
            v.push_back(vertexString(x/2-x/div*i, -y/2, -z/2+z/div*(j+1)));
            v.push_back(vertexString(x/2-x/div*(i+1), -y/2, -z/2+z/div*(j+1)));
            //front
            v.push_back(vertexString(-x/2+x/div*j, y/2-y/div*(i+1), z/2));
            v.push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*i, z/2));
            v.push_back(vertexString(-x/2+x/div*j, y/2-y/div*i, z/2));

            v.push_back(vertexString(-x/2+x/div*j, y/2-y/div*(i+1), z/2));
            v.push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*(i+1), z/2));
            v.push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*i, z/2));
            //back
            v.push_back(vertexString(-x/2+x/div*j, y/2-y/div*(i+1), -z/2));
            v.push_back(vertexString(-x/2+x/div*j, y/2-y/div*i, -z/2));
            v.push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*i, -z/2));

            v.push_back(vertexString(-x/2+x/div*j, y/2-y/div*(i+1), -z/2));
            v.push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*i, -z/2));
            v.push_back(vertexString(-x/2+x/div*(j+1), y/2-y/div*(i+1), -z/2));
            //right
            v.push_back(vertexString(x/2, -y/2+y/div*j, z/2-z/div*(i+1)));
            v.push_back(vertexString(x/2, -y/2+y/div*(j+1), z/2-z/div*i));
            v.push_back(vertexString(x/2, -y/2+y/div*j, z/2-z/div*i));

            v.push_back(vertexString(x/2, -y/2+y/div*j, z/2-z/div*(i+1)));
            v.push_back(vertexString(x/2, -y/2+y/div*(j+1), z/2-z/div*(i+1)));
            v.push_back(vertexString(x/2, -y/2+y/div*(j+1), z/2-z/div*i));
            //left
            v.push_back(vertexString(-x/2, -y/2+y/div*j, z/2-z/div*(i+1)));
            v.push_back(vertexString(-x/2, -y/2+y/div*j, z/2-z/div*i));
            v.push_back(vertexString(-x/2, -y/2+y/div*(j+1), z/2-z/div*i));

            v.push_back(vertexString(-x/2, -y/2+y/div*j, z/2-z/div*(i+1)));
            v.push_back(vertexString(-x/2, -y/2+y/div*(j+1), z/2-z/div*i));
            v.push_back(vertexString(-x/2, -y/2+y/div*(j+1), z/2-z/div*(i+1)));
        }
    }
    return v;
}


std::vector<std::string> sphere(float radius, int slices, int stacks) {
    std::vector<std::string> v;
    double teta, fi;
    for (int i = 0; i < stacks; i++) {
        for (int j = 0; j < slices; j++) {
            if (i != stacks - 1) {
                fi = i * M_PI/stacks;
                teta = j * 2 * M_PI/slices;
                v.push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta)*sin(fi)));
                fi = (i+1) * M_PI/stacks;
                teta = (j+1) * 2 * M_PI/slices;
                v.push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta)*sin(fi)));
                teta = j * 2 * M_PI/slices;
                v.push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta)*sin(fi)));
            }
            if (i != 0) {
                fi = i * M_PI/stacks;
                teta = j * 2 * M_PI/slices;
                v.push_back(vertexString(radius*cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta)*sin(fi)));
                teta = (j + 1) * 2 * M_PI/slices;
                v.push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta) * sin(fi)));
                fi = (i + 1) * M_PI/stacks;
                v.push_back(vertexString(radius * cos(teta)*sin(fi),
                            radius * cos(fi),
                            radius * sin(teta) * sin(fi)));
            }
        }
    }
    return v;
}


std::vector<std::string> cylinder(float radB, float radT, float height, int slices, int stacks) {
    std::vector<std::string> v;
    float alpha = (float)2* M_PI / slices;
    float h = (float)height / stacks;
    float angle;                            //ângulo formado pela linha do raio do ponto em que estamos e o eixo dos z's
    float lvl;                              //recebe o valor do raio em função das stacks percorridas
    float prev_lvl;                         //recebe o valor anterior do raio em função das stacks percorridas
    //draw base
    for (int j = 1; j <= slices; j++) {
        v.push_back(vertexString(sin(alpha*(j-1))*radB,
                    0.0f, cos(alpha*(j-1))*radB));
        angle = alpha * j;
        v.push_back(vertexString(0.0f, 0.0f, 0.0f));
        v.push_back(vertexString(sin(angle)*radB, 0.0f, cos(angle)*radB));
        prev_lvl = radB;
        //draw sides
        for (int i = 1; i <= stacks; i++){
			lvl = radB - (float)(radB - radT)*i / stacks;
			v.push_back(vertexString(prev_lvl * sin(alpha*(j - 1)),
				h*(i - 1), prev_lvl * cos(alpha*(j - 1))));
			v.push_back(vertexString(prev_lvl * sin(angle),
				h*(i - 1), prev_lvl * cos(angle)));
			v.push_back(vertexString(lvl * sin(alpha*(j - 1)),
				h * i, lvl * cos(alpha * (j - 1))));
			v.push_back(vertexString(lvl * sin(alpha*(j - 1)),
				h * i, lvl * cos(alpha * (j - 1))));
			v.push_back(vertexString(prev_lvl * sin(angle),
				h*(i - 1), prev_lvl * cos(angle)));
			v.push_back(vertexString(lvl * sin(angle),
				h*i, lvl * cos(angle)));
			prev_lvl = lvl;
        }
    }
    //draw top
    if (radT) {
        for (int j = 1; j <= slices; j++) {
            angle = alpha * j;
            v.push_back(vertexString(sin(angle)*radT, height, cos(angle)*radT));
            angle = alpha * (j - 1);
            v.push_back(vertexString(0.0f, height, 0.0f));
            v.push_back(vertexString(sin(angle)*radT, height, cos(angle)*radT));
        }
    }
    return v;
}


std::vector<std::string> torus(float inner, float outer, int sides, int rings) {
    float alpha = (float) 2 * M_PI / rings;
    float r = outer - inner;
    float beta = (float) 2 * M_PI / sides;
    float angle1, angle2;
    std::vector<std::string> v;
    for (int i = 1; i <= rings; i++) {
        for (int j = 1; j <= sides; j++) {
            angle1 = alpha * (i - 1);
            angle2 = beta * (j - 1);
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        r * sin(angle2),
                        cos(angle1)*(outer - r*cos(angle2))));
            angle2 = beta * j;
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        r * sin(angle2),
                        cos(angle1)*(outer - r*cos(angle2))));
            angle2 = beta * (j-1);
            angle1 = alpha * i;
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        r * sin(angle2),
                        cos(angle1)*(outer - r*cos(angle2))));
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        r * sin(angle2),
                        cos(angle1)*(outer - r*cos(angle2))));
            angle1 = alpha * (i - 1);
            angle2 = beta * j;
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        r * sin(angle2),
                        cos(angle1)*(outer - r*cos(angle2))));
            angle1 = alpha * i;
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        r * sin(angle2),
                        cos(angle1)*(outer - r*cos(angle2))));
        }
    }
    return v;
}

std::vector<std::string> ruby(float rb, float rt, float heightb, float heightt, int slices, int stacks) {
    int i, j;
    float alpha = (float)2 * M_PI / slices;
    float h = (float)heightb / stacks;
    float angle;                            //ângulo formado pela linha do raio do ponto em que estamos e o eixo dos z's
    float lvl;                              //recebe o valor do raio em função das stacks percorridas
    float prev_lvl;                         //recebe o valor anterior do raio em função das stacks percorridas
    std::vector<std::string> v;
    v = cylinder(rb, rt, heightt, slices, stacks);
    for (j = 1; j <= slices; j++) {
        angle = alpha * j;
        prev_lvl = rb;
        //draw sides
        for (i = 1; i <= stacks; i++) {
            lvl = rb - (float)rb*i / stacks;
			v.push_back(vertexString(prev_lvl * sin(angle),
				-h*(i - 1), prev_lvl * cos(angle)));
            v.push_back(vertexString(prev_lvl * sin(alpha*(j - 1)),
                        -h*(i - 1), prev_lvl * cos(alpha*(j - 1))));
            v.push_back(vertexString(lvl * sin(alpha*(j - 1)),
                        -h * i, lvl * cos(alpha * (j - 1))));
            v.push_back(vertexString(lvl * sin(alpha*(j - 1)),
                        -h * i, lvl * cos(alpha * (j - 1))));
			v.push_back(vertexString(lvl * sin(angle),
				-h * i, lvl * cos(angle)));
			v.push_back(vertexString(prev_lvl * sin(angle),
                        -h * (i - 1), prev_lvl * cos(angle)));
            prev_lvl = lvl;
        }
    }
    return v;
}


std::vector<std::string> crown(float inner, float outer, int sides) {
    float alpha = (float) 2 * M_PI / sides;
    float r = outer - inner;
    float beta = (float) 2 * M_PI / sides;
    float angle1, angle2;
    std::vector<std::string> v;
    for (int i = 1; i <= sides; i++) {
        for (int j = 1; j <= sides; j++) {
            angle1 = alpha * (i - 1);
            angle2 = beta * (j - 1);
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        0,
                        cos(angle1)*(outer - r*cos(angle2))));
            angle2 = beta * j;
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        0,
                        cos(angle1)*(outer - r*cos(angle2))));
            angle2 = beta * (j-1);
            angle1 = alpha * i;
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        0,
                        cos(angle1)*(outer - r*cos(angle2))));
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        0,
                        cos(angle1)*(outer - r*cos(angle2))));
            angle1 = alpha * (i - 1);
            angle2 = beta * j;
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        0,
                        cos(angle1)*(outer - r*cos(angle2))));
            angle1 = alpha * i;
            v.push_back(vertexString(sin(angle1)*(outer - r*cos(angle2)),
                        0,
                        cos(angle1)*(outer - r*cos(angle2))));
        }
    }
    return v;
}
