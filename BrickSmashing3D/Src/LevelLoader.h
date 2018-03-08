#pragma once
#include "vendor/Binary-Reader-Writer/BinaryRW.h"
#include "Platform.h"
#include <fstream>


struct SceneFile
{
	std::vector<Platform*> platforms;
};

class LevelLoader {
private:

public:
	LevelLoader()
	{

	}


	void Write(const std::string& filePath, const std::vector<Platform*> platforms)
	{



		BinaryWriter writer = BinaryWriter();

		writer.writeInt32_LE(platforms.size());

		for (int i = 0; i < platforms.size(); i++)
		{
			writer.writeFloat_LE(platforms[i]->GetLength());
			writer.writeFloat_LE(platforms[i]->GetHeight());


			writer.writeFloat_LE(platforms[i]->GetPosition().x);
			writer.writeFloat_LE(platforms[i]->GetPosition().y);
			writer.writeFloat_LE(platforms[i]->GetPosition().z);
		}

		std::ofstream output(filePath, std::ios::binary);
		output.write((const char*)&writer.getBuffer()[0], writer.getBuffer().size());
	}

	void Load(const std::string& filePath, SceneFile& sceneFile, q3Scene& physicsScene)
	{
		std::ifstream input(filePath, std::ios::binary);
		std::vector<unsigned char> buffer((
			std::istreambuf_iterator<char>(input)),
			(std::istreambuf_iterator<char>()));

		BinaryReader reader(buffer);

		auto platformCount = reader.readInt32_LE();

		for (int i = 0; i < platformCount; i++)
		{
			const auto length = reader.readFloat_LE();
			const auto height = reader.readFloat_LE();
			Platform* plat = new Platform(length, height, physicsScene);


			auto x = reader.readFloat_LE();
			auto y = reader.readFloat_LE();
			auto z = reader.readFloat_LE();

			plat->SetPosition({ x,y,z});

			sceneFile.platforms.push_back(plat);
		}
	}

};
