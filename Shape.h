#include<iostream>
#include<glew.h>
#include<glfw3.h>
#include<math.h>
#include "glm/ext.hpp"
using namespace std;

const float PI = 3.14159265359;
const float DEG_TO_RAD = PI / 180.0;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

class Matrix {
public:
	float matrix[4][4];
	Matrix() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
				matrix[i][j] = 0;
		}
	}
	Matrix RotationX(float degree) {
		Matrix result;
		float radian = degree * DEG_TO_RAD;
		result.matrix[0][0] = 1;
		result.matrix[1][1] = cos(radian);
		result.matrix[1][2] = -1 * sin(radian);
		result.matrix[2][1] = sin(radian);
		result.matrix[2][2] = cos(radian);
		result.matrix[3][3] = 1;
		return result;
	}
	Matrix RotationY(float degree) {
		Matrix result;
		float radian = degree * DEG_TO_RAD;
		result.matrix[0][0] = cos(radian);
		result.matrix[0][2] = sin(radian);
		result.matrix[1][1] = 1;
		result.matrix[2][0] = -sin(radian);
		result.matrix[2][2] = cos(radian);
		result.matrix[3][3] = 1;
		return result;
	}
	Matrix RotationZ(float degree) {
		Matrix result;
		float radian = degree * DEG_TO_RAD;
		result.matrix[0][0] = cos(radian);
		result.matrix[0][1] = -sin(radian);
		result.matrix[1][0] = sin(radian);
		result.matrix[1][1] = cos(radian);
		result.matrix[2][2] = 1;
		result.matrix[3][3] = 1;
		return result;
	}
	Matrix operator+(Matrix another) {
		Matrix result;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
				result.matrix[i][j] = matrix[i][j] + another.matrix[i][j];
		}
		return result;
	}
	Matrix operator-(Matrix another) {
		Matrix result;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
				result.matrix[i][j] = matrix[i][j] - another.matrix[i][j];
		}
		return result;
	}
	Matrix operator*(Matrix another) {
		Matrix result;
		float sum = 0.0f;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++)
					sum += (matrix[i][k] * another.matrix[k][j]);
				result.matrix[i][j] = sum;
				sum = 0.0f;
			}
		}
		return result;
	}
	Matrix TranslationMatrix(float x, float y, float z) {
		Matrix matrix_translation;
		matrix_translation.matrix[0][3] = x;
		matrix_translation.matrix[1][3] = y;
		matrix_translation.matrix[2][3] = z;
		matrix_translation.matrix[3][3] = 1;
		return matrix_translation;
	}
	void ResetMatrix() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
				matrix[i][j] = 0;
		}
	}
	void ShowMatrix() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++)
				cout << matrix[i][j] << " ";
			cout << endl;
		}
	}
	Matrix Scalling(float x, float y, float z) {
		Matrix result;
		result.matrix[0][0] = x;
		result.matrix[1][1] = y;
		result.matrix[2][2] = z;
		return result;
	}
};

class Vertex {
public:
	GLfloat x, y, z;
	Vertex(GLfloat _x = 0, GLfloat _y = 0, GLfloat _z = 0) {
		x = _x;
		y = _y;
		z = _z;
	}
	Vertex operator-(const Vertex& vertex) {
		Vertex temp(x, y, z);
		temp.x -= vertex.x;
		temp.y -= vertex.y;
		temp.z -= vertex.z;
		return temp;
	}
	Vertex operator-(float input) {
		Vertex temp;
		temp.x = x - input;
		temp.y = y - input;
		temp.z = z - input;
		return temp;
	}
	Vertex operator+(const Vertex& vertex) {
		Vertex temp(x, y, z);
		temp.x += vertex.x;
		temp.y += vertex.y;
		temp.z += vertex.z;
		return temp;
	}
	Vertex operator+(Matrix input) {
		Vertex v;
		v.x = this->x + input.matrix[0][3];
		v.y = this->y + input.matrix[1][3];
		v.z = this->z + input.matrix[2][3];
		return v;
	}
	Vertex operator+(float input) {
		Vertex temp;
		temp.x = x + input;
		temp.y = y + input;
		temp.z = z + input;
		return temp;
	}
	Vertex operator*(const Vertex& Another) {
		Vertex temp;
		temp.x = x * Another.x;
		temp.y = y * Another.y;
		temp.z = z * Another.z;
		return temp;
	}
	Vertex operator*(float number) {
		Vertex temp(x, y, z);
		temp.x *= number;
		temp.y *= number;
		temp.z *= number;
		return temp;
	}
	Vertex operator/(float input) {
		Vertex temp;
		temp.x = x / input;
		temp.y = y / input;
		temp.z = z / input;
		return temp;
	}
	void operator=(const Vertex& vertex) {
		x = vertex.x;
		y = vertex.y;
		z = vertex.z;
	}
	Vertex normalize() {
		float length = pow(pow(x, 2.0) + pow(y, 2.0) + pow(z, 2.0), 0.5);
		x /= length;
		y /= length;
		z /= length;
		return Vertex(x, y, z);
	}
	float Dot(Vertex input) {
		return (x * input.x) + (y * input.y) + (z * input.z);
	}
	Vertex Cross(Vertex input) {
		Vertex temp;
		temp.x = x * input.x;
		temp.y = y * input.y;
		temp.z = z * input.z;
		return temp;
	}
	Vertex operator*(Matrix TransformationMatrix) {
		Vertex result;
		result.x = (TransformationMatrix.matrix[0][0] * x) + (TransformationMatrix.matrix[0][1] * y) + (TransformationMatrix.matrix[0][2] * z);
		result.y = (TransformationMatrix.matrix[1][0] * x) + (TransformationMatrix.matrix[1][1] * y) + (TransformationMatrix.matrix[1][2] * z);
		result.z = (TransformationMatrix.matrix[2][0] * x) + (TransformationMatrix.matrix[2][1] * y) + (TransformationMatrix.matrix[2][2] * z);
		return result;
	}
	Vertex Scalling(Matrix Another) {
		Vertex v;
		v.x = this->x * Another.matrix[0][0];
		v.y = this->y * Another.matrix[1][1];
		v.z = this->z * Another.matrix[2][2];
		return v;
	}
};

Vertex getRotationResult(const Vertex& pivot, const Vertex& vector, float angle, Vertex point, bool isEuler = false) {
	Vertex temp, newPosition;
	temp = point - pivot;
	if (isEuler)
		temp = point;
	else
		temp = point - pivot;

	newPosition.x = temp.x * (cos(angle) + pow(vector.x, 2.0f) * (1.0f - cos(angle))) + temp.y * (vector.x * vector.y * (1.0f - cos(angle)) - vector.z * sin(angle)) + temp.z * (vector.x * vector.z * (1.0f - cos(angle)) + vector.y * sin(angle));
	newPosition.y = temp.x * (vector.x * vector.y * (1.0f - cos(angle)) + vector.z * sin(angle)) + temp.y * (cos(angle) + pow(vector.y, 2.0f) * (1.0f - cos(angle))) + temp.z * (vector.y * vector.z * (1.0f - cos(angle)) - vector.x * sin(angle));
	newPosition.z = temp.x * (vector.x * vector.z * (1.0f - cos(angle)) - vector.y * sin(angle)) + temp.y * (vector.y * vector.z * (1.0f - cos(angle)) + vector.x * sin(angle)) + temp.z * (cos(angle) + pow(vector.z, 2.0f) * (1.0f - cos(angle)));

	if (isEuler)
		temp = newPosition;
	else
		temp = newPosition + pivot;

	temp = newPosition + pivot;
	return temp;
}

void getRotationMatrix(const Vertex& vector, float angle, Vertex* matrix) {
	matrix[0].x = (cos(angle) + pow(vector.x, 2.0f) * (1.0f - cos(angle)));
	matrix[0].y = (vector.x * vector.y * (1.0f - cos(angle)) - vector.z * sin(angle));
	matrix[0].z = (vector.x * vector.z * (1.0f - cos(angle)) + vector.y * sin(angle));

	matrix[1].x = (vector.x * vector.y * (1.0f - cos(angle)) + vector.z * sin(angle));
	matrix[1].y = (cos(angle) + pow(vector.y, 2.0f) * (1.0f - cos(angle)));
	matrix[1].z = (vector.x * vector.z * (1.0f - cos(angle)) - vector.x * sin(angle));

	matrix[2].x = (vector.x * vector.z * (1.0f - cos(angle)) - vector.y * sin(angle));
	matrix[2].y = (vector.y * vector.z * (1.0f - cos(angle)) + vector.x * sin(angle));
	matrix[2].z = (cos(angle) + pow(vector.z, 2.0f) * (1.0f - cos(angle)));
}

int getPascal(int row, int col) {
	if (col > row)
		return 0;
	else if (col == 0 || row == 0)
		return 1;
	return getPascal(row - 1, col - 1) + getPascal(row - 1, col);
}

class Color {
public:
	float r, g, b;
	Color(float _r = 0, float _g = 0, float _b = 0) {
		r = _r;
		g = _g;
		b = _b;
	}
	Color operator+(Color input) {
		Color temp;
		temp.r = r + input.r;
		temp.g = g + input.g;
		temp.b = b + input.b;
		return temp;
	}
	Color operator+(float x) {
		Color temp;
		temp.r = r + x;
		temp.g = g + x;
		temp.b = b + x;
		return temp;
	}
	Color operator*(float x) {
		Color temp;
		temp.r = r * x;
		temp.g = g * x;
		temp.b = b * x;
		return temp;
	}
	Color operator*(Color x) {
		Color temp;
		temp.r = r * x.r;
		temp.g = g * x.g;
		temp.b = b * x.b;
		return temp;
	}
};

class Material {
public:
	Color color;
	float ka, kd, ks;
	Material() {
		ka = 0.5;
		kd = 0.4;
		ks = 1.0;
	}
	void setColor(Color input) {
		color.r = input.r;
		color.g = input.g;
		color.b = input.b;
	}
	Color getAmbient() {
		return color * ka;
	}
	Color getDifuse(Vertex a, Vertex b, Vertex c, Vertex l) {
		Vertex v1 = a - b;
		Vertex v2 = c - b;
		Vertex L, N;
		L = (l - b).normalize();
		N = (v1 * v2).normalize();
		double pl = N.Dot(L);
		if (pl < 0)
			pl = -pl;
		return color * kd * pl;
	}
	Color getSpecular(Vertex a, Vertex b, Vertex c, Vertex l, Vertex eye) {
		Vertex v1 = a - b;
		Vertex v2 = c - b;
		Vertex L, n;
		L = (l - b).normalize();
		n = (v1 * v2).normalize();
		double pl = n.Dot(L);
		if (pl < 0)
			pl = 0;
		Vertex r = (n * (2 * pl)) - L;
		Vertex mata;
		double temp = r.Dot((mata - b).normalize());
		if (temp < 0)
			temp = 0;
		return color * temp * ks;
	}
};

class Mesh {
	vector<Vertex> vertices, normals, uvs, vertices_draw;
	vector<unsigned int> VertexIndices, uvIndices, NormalIndices;
	GLuint buffer, uvbuffer, colorbuffer;
	Matrix Transformation_Matrix;
	Material material;
	vector<Color> Colors;
	GLuint MatrixID;
	ShaderMesh loadshaders;
	glm::mat4 mvp;
public:
	Mesh(const char* Obj_path = "") {
		FILE* objFile = fopen(Obj_path, "r");
		if (!objFile)
			return;
		vector<Vertex> temp_vertices, temp_uvs, temp_normals;
		while (true) {
			char lines[128];
			int res = fscanf(objFile, "%s", lines);
			if (res == EOF)
				break;
			if (strcmp(lines, "v") == 0) {
				Vertex v;
				fscanf(objFile, "%f %f %f\n", &v.x, &v.y, &v.z);
				temp_vertices.push_back(v);
				cout << "V : " << v.x << " " << v.y << " " << v.z << endl;
			}
			else if (strcmp(lines, "vn") == 0) {
				Vertex normal;
				fscanf(objFile, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
				cout << "VN : " << normal.x << " " << normal.y << " " << normal.z << endl;
			}
			else if (strcmp(lines, "vt") == 0) {
				Vertex uv; uv.z = 0;
				fscanf(objFile, "%f %f\n", &uv.x, &uv.y);
				temp_uvs.push_back(uv);
				cout << "VT : " << uv.x << " " << uv.y << " " << uv.z << endl;
			}
			else if (strcmp(lines, "f") == 0) {
				string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				if (fscanf(objFile, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2])) {
					VertexIndices.push_back(vertexIndex[0]);
					VertexIndices.push_back(vertexIndex[1]);
					VertexIndices.push_back(vertexIndex[2]);
					uvIndices.push_back(uvIndex[0]);
					uvIndices.push_back(uvIndex[1]);
					uvIndices.push_back(uvIndex[2]);
					NormalIndices.push_back(normalIndex[0]);
					NormalIndices.push_back(normalIndex[1]);
					NormalIndices.push_back(normalIndex[2]);
					//Menampilkan Text yang rapi
					printf("F : %3d/%3d/%3d%10d/%3d/%3d%10d/%3d/%3d\n", vertexIndex[0], vertexIndex[1], vertexIndex[2], uvIndex[0], uvIndex[1], uvIndex[2], normalIndex[0], normalIndex[1], normalIndex[2]);
				}

			}
		}
		for (unsigned int i = 0; i < VertexIndices.size(); i++) {
			unsigned int vertexIndex = VertexIndices[i];
			unsigned int uvIndex = uvIndices[i];
			unsigned int normalIndex = NormalIndices[i];

			Vertex vertex = temp_vertices[vertexIndex - 1] / 20;
			Vertex uv = temp_uvs[uvIndex - 1];
			Vertex normal = temp_normals[normalIndex - 1];

			vertices.push_back(vertex);
			uvs.push_back(uv);
			normals.push_back(normal);
		}
		vertices_draw = vertices;
		fclose(objFile);
	}

	void PrintPoints() {
		for (int j = 0; j < vertices.size(); j++)
			cout << vertices[j].x << " " << vertices[j].y << " " << vertices[j].z << endl;
		cout << "Vertex size: " << vertices.size() << endl;
	}

	void BindBuffer() {
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	void Initialize_Object() {
		glGenBuffers(1, &buffer);
		setArrayBuffers();
	}

	void Initialize_UVS() {
		glGenBuffers(1, &uvbuffer);
		setUVSBuffers();
	}

	void Initialize_Combination_Shading(Color color) {
		glGenBuffers(1, &colorbuffer);
		material.setColor(color);
		Color final_color, ambient, diffuse, specular;
		for (int j = 0; j < vertices.size(); j += 3) {
			ambient = material.getAmbient();
			specular = material.getSpecular(vertices[j], vertices[j + 1],
				vertices[j + 2], Vertex(0, 0.6, 0), Vertex(0, 0.4, 0));
			diffuse = material.getDifuse(vertices[j], vertices[j + 1],
				vertices[j + 2], Vertex(0, 0.6, 0));
			final_color = ambient + specular + diffuse;
			Colors.push_back(final_color);
			Colors.push_back(final_color);
			Colors.push_back(final_color);
		}
		setColorBuffers();
	}

	void Initialize_Specular_Shading(Color color) {
		glGenBuffers(1, &colorbuffer);
		material.setColor(color);
		Color final_color, ambient, diffuse, specular;
		for (int j = 0; j < vertices.size(); j += 3) {
			specular = material.getSpecular(vertices[j], vertices[j + 1],
				vertices[j + 2], Vertex(0, 0.6, 0), Vertex(0, 0.4, 0));
			Colors.push_back(specular);
			Colors.push_back(specular);
			Colors.push_back(specular);
		}
		setColorBuffers();
	}

	void Initialize_Diffuse_Shading(Color color) {
		glGenBuffers(1, &colorbuffer);
		material.setColor(color);
		Color final_color, ambient, diffuse, specular;
		for (int j = 0; j < vertices.size(); j += 3) {
			diffuse = material.getDifuse(vertices[j], vertices[j + 1],
				vertices[j + 2], Vertex(0, 0.6, 0));
			Colors.push_back(diffuse);
			Colors.push_back(diffuse);
			Colors.push_back(diffuse);
		}
		setColorBuffers();
	}

	void Initialize_Ambient_Shading(Color color) {
		glGenBuffers(1, &colorbuffer);
		material.setColor(color);
		Color final_color, ambient, diffuse, specular;
		for (int j = 0; j < vertices.size(); j += 3) {
			ambient = material.getAmbient();
			Colors.push_back(ambient);
			Colors.push_back(ambient);
			Colors.push_back(ambient);
		}
		setColorBuffers();
	}

	void setUVSBuffers() {
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(Vertex), uvs.data(), GL_STATIC_DRAW);
	}

	void setArrayBuffers() {
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices_draw.data(), GL_STATIC_DRAW);
	}

	void initialize_shader(const char* vertex_path, const char* fragment_path) {
		loadshaders.LoadShaders(vertex_path, fragment_path);
	}

	void setUpdateColor() {
		Color final_color, ambient, diffuse, specular;
		for (int j = 0; j < vertices.size(); j += 3) {
			ambient = material.getAmbient();
			specular = material.getSpecular(vertices_draw[j], vertices_draw[j + 1],
				vertices_draw[j + 2], Vertex(0, 0.6, 0), Vertex(0, 0.4, 0));
			diffuse = material.getDifuse(vertices_draw[j], vertices_draw[j + 1],
				vertices_draw[j + 2], Vertex(0, 0.6, 0));
			final_color = ambient + diffuse + specular;
			Colors[j] = final_color;
			Colors[j + 1] = final_color;
			Colors[j + 2] = final_color;
		}
	}

	void setColorBuffers() {
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glBufferData(GL_ARRAY_BUFFER, Colors.size() * sizeof(Vertex), Colors.data(), GL_STATIC_DRAW);
	}

	void DrawObject() {
		loadshaders.use();
		BindBuffer();
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	}

	void Scaling(float input) {
		for (int j = 0; j < vertices.size(); j++)
			vertices_draw[j] = vertices_draw[j] * input;
		setArrayBuffers();
	}

	void TranslateX(float input) {
		Transformation_Matrix = Transformation_Matrix.TranslationMatrix(input, 0, 0);
		for (int j = 0; j < vertices.size(); j++)
			vertices_draw[j] = vertices_draw[j] + Transformation_Matrix;
		setArrayBuffers();
		setUpdateColor();
		setColorBuffers();
	}

	void TranslateY(float input) {
		Transformation_Matrix = Transformation_Matrix.TranslationMatrix(0, input, 0);
		for (int j = 0; j < vertices.size(); j++)
			vertices_draw[j] = vertices_draw[j] + Transformation_Matrix;
		setArrayBuffers();
		setUpdateColor();
		setColorBuffers();
	}

	void TranslateZ(float input) {
		Transformation_Matrix = Transformation_Matrix.TranslationMatrix(0, 0, input);
		for (int j = 0; j < vertices.size(); j++)
			vertices_draw[j] = vertices_draw[j] + Transformation_Matrix;
		setArrayBuffers();
		setUpdateColor();
		setColorBuffers();
	}

	void RotationX(float degree) {
		Transformation_Matrix = Transformation_Matrix.RotationX(degree);
		for (int j = 0; j < vertices.size(); j++)
			vertices_draw[j] = vertices_draw[j] * Transformation_Matrix;
		setArrayBuffers();
	}

	void RotationY(float degree) {
		Transformation_Matrix = Transformation_Matrix.RotationY(degree);
		for (int j = 0; j < vertices.size(); j++)
			vertices_draw[j] = vertices_draw[j] * Transformation_Matrix;
		setArrayBuffers();
	}

	void RotationZ(float degree) {
		Transformation_Matrix = Transformation_Matrix.RotationZ(degree);
		for (int j = 0; j < vertices.size(); j++)
			vertices_draw[j] = vertices_draw[j] * Transformation_Matrix;
		setArrayBuffers();
	}

};