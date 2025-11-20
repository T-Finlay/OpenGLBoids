#include "BoidsManager.h"
#include <random>

BoidsManager::BoidsManager(GLuint* ptr) {
	boidRenderBufferPointer = ptr;
}

void BoidsManager::initialise(std::shared_ptr<Entity> self) {
	computeShader.reset(new ComputeShader("boids.comp"));
	GLuint buffers[2];
	glGenBuffers(2, buffers);
	readingBuffer = buffers[0];
	writingBuffer = buffers[1];
	std::unique_ptr<BoidData[]> boidDatas = initialiseBoidsData();

	//buffers must be bound at least once to actually be created on the GPU by opengl
	glBindBuffer(GL_ARRAY_BUFFER, readingBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, writingBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Note the DYNAMIC_DRAW usage enum, data is frequently modified and used by the vertex shader,
	//so this enum is appropriate
	glNamedBufferData(readingBuffer, NUM_BOIDS * sizeof(BoidData), boidDatas.get(), GL_DYNAMIC_DRAW);
	glNamedBufferData(writingBuffer, NUM_BOIDS * sizeof(BoidData), boidDatas.get(), GL_DYNAMIC_DRAW);

	computeShader->useShader();
	glUniform1ui(computeShader->uniformLocation("numBoids"), (unsigned int)NUM_BOIDS);
	glUniform1f(computeShader->uniformLocation("coheasionBias"), 1.f);
	glUniform1f(computeShader->uniformLocation("separationBias"), 1.f);
	glUniform1f(computeShader->uniformLocation("alignmentBias"), 1.f);
	glUniform1f(computeShader->uniformLocation("homingBias"), 0.5f);
	glUniform1f(computeShader->uniformLocation("maintainanceBias"), 0.1f);
	glUniform1f(computeShader->uniformLocation("boidRange"), MAX_SINGLE_AXIS_DIST_FROM_ORIGIN);
	glUniform1f(computeShader->uniformLocation("boidProximityRange"), 1.f);
	glUniform1f(computeShader->uniformLocation("boidDetectionRange"), 10.f);
	glUniform1f(computeShader->uniformLocation("boidSpeedPerSecond"), 3.f);
}

void BoidsManager::update(float deltaTime, std::shared_ptr<Entity> self) {
	computeShader->useShader();
	BoidData testbd[NUM_BOIDS];

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, readingBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, readingBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, writingBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, writingBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	glUniform1f(computeShader->uniformLocation("deltaTime"), deltaTime);
	glGetNamedBufferSubData(writingBuffer, 0, (NUM_BOIDS * sizeof(BoidData)), &testbd);
	glDispatchCompute(8, 8, 2); //must multiply to equal NUM_BOIDS
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	
	auto a = sizeof(BoidData);
	//test read data back to memory
	glGetNamedBufferSubData(writingBuffer,0,(NUM_BOIDS * sizeof(BoidData)),&testbd);
	2;
	*boidRenderBufferPointer = writingBuffer;
	swapBuffers();
}

void BoidsManager::swapBuffers() {
	GLuint temp;
	temp = readingBuffer;
	readingBuffer = writingBuffer;
	writingBuffer = temp;
}

std::unique_ptr<BoidData[]> BoidsManager::initialiseBoidsData() {
	std::unique_ptr<BoidData[]> boidData(new BoidData[NUM_BOIDS]);
	std::default_random_engine eng;
	std::uniform_real_distribution<float>
		posRng(-MAX_SINGLE_AXIS_DIST_FROM_ORIGIN, MAX_SINGLE_AXIS_DIST_FROM_ORIGIN);
	std::uniform_real_distribution<float>
		dirRng(-1.f, 1.f);

	for (int i = 0; i < NUM_BOIDS;i++) {
		glm::vec3 pos = glm::vec3(posRng(eng), posRng(eng), posRng(eng));
		glm::vec3 dir = glm::normalize(glm::vec3(dirRng(eng), dirRng(eng), dirRng(eng)));
		boidData.get()[i].pos.x = pos.x;
		boidData.get()[i].pos.y = pos.y;
		boidData.get()[i].pos.z = pos.z;
		boidData.get()[i].pos.w = 0.0f;//padding
		boidData.get()[i].dir.x = dir.x;
		boidData.get()[i].dir.y = dir.y;
		boidData.get()[i].dir.z = dir.z;
		boidData.get()[i].dir.w = 0.0f;//padding
	}
	return boidData;
}
