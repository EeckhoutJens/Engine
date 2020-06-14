#include "MiniginPCH.h"
#include "CollisionComponent.h"
#include "TextureComponent.h"
#include "GameObject.h"

NFE::CollisionComponent::CollisionComponent(dae::GameObject* parent) : BaseComponent(parent)
{
}

void NFE::CollisionComponent::Update(const float&, float, float, float, float)
{
	SetCollisionRect(*getParent()->GetComponent<NFE::TextureComponent>()->GetTextureData());
	CheckCollision();
}

void NFE::CollisionComponent::SetCollisionRect(SDL_Rect colRect)
{
	RectPoints controlPoints{};
	controlPoints.BL.x = float(colRect.x);
	controlPoints.BL.y = float(colRect.y);

	controlPoints.TL.x = float(colRect.x);
	controlPoints.TL.y = float(colRect.y + colRect.h);

	controlPoints.TR.x = float(colRect.x + colRect.w);
	controlPoints.TR.y = float(colRect.y + colRect.h);

	controlPoints.BR.x = float(colRect.x + colRect.w);
	controlPoints.BR.y = float(colRect.y);

	m_CollisionRectPoints = controlPoints;
}


void NFE::CollisionComponent::CheckCollision()
{
	for (CollisionComponent* pComp : m_VecCollComps)
	{
		if (m_Type == CollisionType::Platform)
		{
			if (pComp->m_Type == m_Type)
			{
				continue;
			}
		}
		auto colRect = pComp->GetCollisionrect();
		
			//CHECK LEFT COLLISION
			//********************
			if (colRect.BL.x + 2.5f > m_CollisionRectPoints.BL.x
				&& colRect.BL.x < m_CollisionRectPoints.BR.x
				&& colRect.TL.y - 5.f > m_CollisionRectPoints.BL.y
				&& colRect.BL.y < m_CollisionRectPoints.BL.y)
			{

				if (m_Type == CollisionType::Platform 
					|| m_Type == CollisionType::Wall
					|| pComp->m_Type == CollisionType::Pickup)
				{
					if (pComp->m_Type == CollisionType::Enemy
						|| pComp->m_Type == CollisionType::Player)
					{
						dae::GameObject* obj = pComp->getParent();
						if (obj)
						{
							auto currPos = obj->GetPosition();
 							obj->SetPosition(m_CollisionRectPoints.BR.x, currPos.y);
						}
						continue;
					}

					if (pComp->m_Type == CollisionType::Bubble ||
						pComp->m_Type == CollisionType::FireBall)
					{
						pComp->m_IsColliding = true;
					}
				}

				if (m_Type == CollisionType::Enemy)
				{
					if (pComp->m_Type == CollisionType::Bubble)
					{
						m_IsColliding = true;
						pComp->m_IsColliding = true;
						continue;
					}

					if (pComp->m_Type == CollisionType::Player)
					{
						pComp->m_IsColliding = true;
					}
				}

					
				if (m_Type == CollisionType::TrappedEnemy)
				{
					if (pComp->m_Type == CollisionType::Player)
					{
						m_HitByPlayer = true;
					}
				}

				if (m_Type == CollisionType::Pickup)
				{
					if (pComp->m_Type == CollisionType::Player)
					{
						m_IsColliding = true;
						pComp->m_IsColliding = true;
					}
				}
				
			}

			if (m_Type == CollisionType::Wall)
			{
				//CHECK RIGHT COLLISION
				//*********************
				if (colRect.BR.x - 5.f > m_CollisionRectPoints.BL.x
					&& colRect.BR.x < m_CollisionRectPoints.BR.x
					&& ((colRect.TL.y - 5.f > m_CollisionRectPoints.BL.y
						&& colRect.BL.y < m_CollisionRectPoints.BL.y)
						))
				{

					if (m_Type == CollisionType::Platform
						|| m_Type == CollisionType::Wall
						|| pComp->m_Type == CollisionType::Pickup)
					{
						if (pComp->m_Type == CollisionType::Enemy
							|| pComp->m_Type == CollisionType::Player)
						{
							dae::GameObject* obj = pComp->getParent();
							auto currPos = obj->GetPosition();
							obj->SetPosition(m_CollisionRectPoints.BL.x - 39.f, currPos.y);
							continue;
						}

						if (pComp->m_Type == CollisionType::Bubble ||
							pComp->m_Type == CollisionType::FireBall)
						{
							pComp->m_IsColliding = true;
						}
					}

				}
			}
			else
			{
				if (colRect.BR.x > m_CollisionRectPoints.BL.x
					&& colRect.BR.x < m_CollisionRectPoints.BR.x
					&& ((colRect.TL.y - 5.f > m_CollisionRectPoints.BL.y
						&& colRect.BL.y < m_CollisionRectPoints.BL.y)
						))
				{

					if (m_Type == CollisionType::Platform
						|| m_Type == CollisionType::Wall
						|| pComp->m_Type == CollisionType::Pickup)
					{
						if (pComp->m_Type == CollisionType::Enemy
							|| pComp->m_Type == CollisionType::Player)
						{
							dae::GameObject* obj = pComp->getParent();
							auto currPos = obj->GetPosition();
							obj->SetPosition(m_CollisionRectPoints.BL.x - 39.f, currPos.y);
							continue;
						}

						if (pComp->m_Type == CollisionType::Bubble ||
							pComp->m_Type == CollisionType::FireBall)
						{
							pComp->m_IsColliding = true;
						}
					}

					if (m_Type == CollisionType::Enemy)
					{
						if (pComp->m_Type == CollisionType::Bubble)
						{
							m_IsColliding = true;
							pComp->m_IsColliding = true;
							continue;
						}

						if (pComp->m_Type == CollisionType::Player)
						{
							pComp->m_IsColliding = true;
						}
					}


					if (m_Type == CollisionType::TrappedEnemy)
					{
						if (pComp->m_Type == CollisionType::Player)
						{
							m_HitByPlayer = true;
						}
					}

					if (m_Type == CollisionType::Pickup)
					{
						if (pComp->m_Type == CollisionType::Player)
						{
							m_IsColliding = true;
						}
					}

					if (m_Type == CollisionType::Player)
					{
						if (pComp->m_Type == CollisionType::FireBall)
						{
							m_IsColliding = true;
							pComp->m_IsColliding = true;
						}
					}
				}
			}



			//CHECK TOP COLLISION
			//*******************
			if (m_Type != CollisionType::Wall)
			{
				if (colRect.TL.y - 1.f > m_CollisionRectPoints.BL.y
					&& colRect.BL.y < m_CollisionRectPoints.BL.y
					&& ((colRect.BL.x > m_CollisionRectPoints.BL.x
						&& colRect.BL.x < m_CollisionRectPoints.BR.x)
						|| (colRect.BR.x > m_CollisionRectPoints.BL.x
							&& colRect.BR.x < m_CollisionRectPoints.BR.x)))
				{

					if (m_Type == CollisionType::Platform
						|| m_Type == CollisionType::Wall)
					{
						if (pComp->m_Type == CollisionType::Enemy
							|| pComp->m_Type == CollisionType::Player
							||pComp->m_Type == CollisionType::Pickup)
						{
							dae::GameObject* obj = pComp->getParent();
							auto currPos = obj->GetPosition();
							obj->SetPosition(currPos.x, m_CollisionRectPoints.BL.y - 43);
							pComp->m_OnGround = true;
							continue;
						}
					}


					if (m_Type == CollisionType::TrappedEnemy)
					{
						if (pComp->m_Type == CollisionType::Player)
						{
							dae::GameObject* obj = pComp->getParent();
							auto currPos = obj->GetPosition();
							obj->SetPosition(currPos.x, m_CollisionRectPoints.BL.y - 43);
							pComp->m_OnGround = true;
							continue;
						}
					}

					if (m_Type == CollisionType::Enemy)
					{
						if (pComp->m_Type == CollisionType::Bubble)
						{
							m_IsColliding = true;
							pComp->m_IsColliding = true;
							continue;
						}

						if (pComp->m_Type == CollisionType::Player)
						{
							pComp->m_IsColliding = true;
						}
					}

					if (m_Type == CollisionType::Pickup)
					{
						if (pComp->m_Type == CollisionType::Player)
						{
							m_IsColliding = true;
						}
					}


					if (m_Type == CollisionType::Player)
					{
						if (pComp->m_Type == CollisionType::FireBall)
						{
							m_IsColliding = true;
							pComp->m_IsColliding = true;
						}
					}

				}
			}



			//CHECK BOTTOM COLLISION
			//**********************
			if (m_Type != CollisionType::Wall)
			{
				if (colRect.TL.y > m_CollisionRectPoints.TL.y
					&& colRect.BL.y < m_CollisionRectPoints.TL.y

					&& ((colRect.BL.x > m_CollisionRectPoints.BL.x
						&& colRect.BL.x < m_CollisionRectPoints.BR.x)
						|| (colRect.BR.x > m_CollisionRectPoints.BL.x
							&& colRect.BR.x < m_CollisionRectPoints.BR.x)))
				{
					if (pComp->m_Type == CollisionType::TrappedEnemy)
					{
						dae::GameObject* obj = pComp->getParent();
						auto currPos = obj->GetPosition();
						obj->SetPosition(currPos.x, m_CollisionRectPoints.TL.y - 7);
					}

					if (pComp->m_Type == CollisionType::Player)
					{
						if (m_Type == CollisionType::TrappedEnemy)
						{
							m_HitByPlayer = true;
						}
						if (m_Type == CollisionType::Enemy)
						{
							pComp->m_IsColliding = true;
						}
					}

					if (m_Type == CollisionType::Enemy)
					{
						if (pComp->m_Type == CollisionType::Bubble)
						{
							m_IsColliding = true;
							pComp->m_IsColliding = true;
							continue;
						}
					}

					if (m_Type == CollisionType::Pickup)
					{
						if (pComp->m_Type == CollisionType::Player)
						{
							m_IsColliding = true;
						}
					}


					if (m_Type == CollisionType::Player)
					{
						if (pComp->m_Type == CollisionType::FireBall)
						{
							m_IsColliding = true;
							pComp->m_IsColliding = true;
						}
					}
				}
			}
			
		}
}
