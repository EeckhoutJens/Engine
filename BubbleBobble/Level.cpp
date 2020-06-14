#include "Level.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "TypeComponent.h"
#include "ZenChanComponent.h"
#include "MaitaComponent.h"
#include "CollisionComponent.h"

Level::Level(float Width, float Height, int rows, int cols)
{
	m_pGrid = new Grid(Width, Height, rows, cols);
}

Level::Level(BinaryReader& reader, const char* filename)
{
	m_pGrid = new Grid(reader, filename);

	int nrOfEnemies{};
	reader.Read(nrOfEnemies, filename);

	for (int i{}; i < nrOfEnemies; ++i)
	{
		std::shared_ptr<dae::GameObject> readEnemy{ std::make_shared<dae::GameObject>() };

		dae::Transform readTransform{};
		reader.Read(readTransform, filename);

		SDL_Rect readRect{};
		reader.Read(readRect, filename);


		GameObjectType readType{};
		reader.Read(readType, filename);

		NFE::TextureComponent* text{ nullptr };
		NFE::CollisionComponent* coll{ nullptr };

		if (readType == GameObjectType::ZenChan)
		{
			text = new NFE::TextureComponent(readEnemy.get(), "ZenChan.png");
			auto ZCComp = new NFE::ZenChanComponent(readEnemy.get());
			readEnemy->AddComponentToVector(ZCComp);
		}
			

		if (readType == GameObjectType::Maita)
		{
			text = new NFE::TextureComponent(readEnemy.get(), "Maita.png");
			auto MComp = new NFE::MaitaComponent(readEnemy.get());
			readEnemy->AddComponentToVector(MComp);
		}
			
			
		

		text->SetTextureData(readRect);
		NFE::TypeComponent* type = new NFE::TypeComponent(readEnemy.get(), readType);

		coll = new NFE::CollisionComponent(readEnemy.get());
		coll->SetCollisionRect(readRect);
		coll->SetCollisionType(NFE::CollisionType::Enemy);

		readEnemy->SetPosition(readTransform.GetPosition().x, readTransform.GetPosition().y);
		readEnemy->AddComponentToVector(text);
		readEnemy->AddComponentToVector(type);
		readEnemy->AddComponentToVector(coll);
		m_pEnemies.push_back(readEnemy);
	}
}

Level::~Level()
{
	delete m_pGrid;
	m_pGrid = nullptr;

	m_pEnemies.clear();
}

std::shared_ptr<dae::GameObject> Level::CreateNewEnemy(float posX, float posY, GameObjectType type)
{
	std::shared_ptr<dae::GameObject> enemyToAdd = std::make_shared<dae::GameObject>();
	enemyToAdd->SetPosition(posX, posY);
	NFE::TextureComponent* textComp = nullptr;
	if (type == GameObjectType::ZenChan)
		textComp = new NFE::TextureComponent{ enemyToAdd.get(),"ZenChan.png" };


	else if (type == GameObjectType::Maita)
		textComp = new NFE::TextureComponent{ enemyToAdd.get(),"Maita.png" };

	textComp->Update(0, 0, 0, 44, 44);
	NFE::TypeComponent* typeComp = new NFE::TypeComponent{ enemyToAdd.get(), type };
	enemyToAdd->AddComponentToVector(textComp);
	enemyToAdd->AddComponentToVector(typeComp);
	m_pEnemies.push_back(enemyToAdd);

	return enemyToAdd;
}

void Level::WriteToFile(BinaryWriter& writer, const char* filename)
{
	m_pGrid->WriteToFile(writer, filename);
	int size = int(m_pEnemies.size());
	writer.Write(size,filename);
	for (size_t i{}; i < m_pEnemies.size(); ++i)
	{
		m_pEnemies[i]->WriteToFile(writer, filename);
	}


}
