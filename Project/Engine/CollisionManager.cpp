#include "EnginePCH.h"

#include "CollisionManager.h"
#include "Scene.h"
#include "SceneManager.h"

namespace mh
{
	std::bitset<(UINT)define::eLayerType::End> CollisionManager::mLayerCollisionMatrix[(UINT)define::eLayerType::End] = {};
	std::map<UINT64, bool> CollisionManager::mCollisionMap;

	void CollisionManager::Initialize()
	{
	}
	void CollisionManager::Update()
	{
		Scene* scene = SceneManager::GetActiveScene();
		for (UINT row = 0; row < (UINT)define::eLayerType::End; row++)
		{
			for (UINT column = 0; column < (UINT)define::eLayerType::End; column++)
			{
				if (mLayerCollisionMatrix[row][column])
				{
					LayerCollision(scene, (define::eLayerType)row, (define::eLayerType)column);
				}
			}
		}
	}
	void CollisionManager::FixedUpdate()
	{
	}
	void CollisionManager::Render()
	{
	}
	void CollisionManager::CollisionLayerCheck(define::eLayerType _left, define::eLayerType _right, bool _enable)
	{
		int row = 0;
		int column = 0;

		if ((UINT)_left <= (UINT)_right)
		{
			row = (UINT)_left;
			column = (UINT)_right;
		}
		else
		{
			row = (UINT)_right;
			column = (UINT)_left;
		}

		mLayerCollisionMatrix[row][column] = _enable;
	}
	void CollisionManager::LayerCollision(Scene* _scene, define::eLayerType _left, define::eLayerType _right)
	{
		const std::vector<GameObject*>& lefts = _scene->GetGameObjects(_left);
		const std::vector<GameObject*>& rights = _scene->GetGameObjects(_right);

		for (GameObject* left : lefts)
		{
			if (left->GetState() != GameObject::eState::Active)
				continue;
			if (left->GetComponent<ICollider2D>() == nullptr)
				continue;

			for (GameObject* right : rights)
			{
				if (right->GetState() != GameObject::eState::Active)
					continue;
				if (right->GetComponent<ICollider2D>() == nullptr)
					continue;
				/*if (left == right) //지워도 상관없어서 주석처리 (오류나면 수정)
					continue;*/

				ColliderCollision(left->GetComponent<ICollider2D>(), right->GetComponent<ICollider2D>());
			}

			/*if ((UINT)left == (UINT)right)  
				break;*/
		}

	}

	void CollisionManager::ColliderCollision(ICollider2D* _left, ICollider2D* _right)
	{
		// 두 충돌체 레이어로 구성된 ID 확인
		union_ColliderID colliderID;
		colliderID.Left = (UINT)_left->GetID();
		colliderID.Right = (UINT)_right->GetID();

		// 이전 충돌 정보를 검색한다.
		// 만약에 충돌정보가 없는 상태라면
		// 충돌정보를 생성해준다.
		std::map<UINT64, bool>::iterator iter = mCollisionMap.find(colliderID.Id);
		if (iter == mCollisionMap.end())
		{
			mCollisionMap.insert(std::make_pair(colliderID.Id, false));
			iter = mCollisionMap.find(colliderID.Id);
		}

		// 충돌체크를 해준다.
		if (Intersect(_left, _right)) // 충돌을 한 상태
		{
			// 최초 충돌중 Enter
			if (iter->second == false)
			{
				if (_left->IsTriiger())
					_left->OnTriggerEnter(_right);
				else
					_left->OnCollisionEnter(_right);

				if (_right->IsTriiger())
					_right->OnTriggerEnter(_left);
				else
					_right->OnCollisionEnter(_left);

				iter->second = true;
			}
			else // 충돌 중이지 않은 상태 Enter
			{
				if (_left->IsTriiger())
					_left->OnTriggerStay(_right);
				else
					_left->OnCollisionStay(_right);

				if (_right->IsTriiger())
					_right->OnTriggerStay(_left);
				else
					_right->OnCollisionStay(_left);
			}
		}
		else // 충돌하지 않은상태
		{
			// 충돌 중인상태 Exit
			if (iter->second == true)
			{
				if (_left->IsTriiger())
					_left->OnTriggerExit(_right);
				else
					_left->OnCollisionExit(_right);

				if (_right->IsTriiger())
					_right->OnTriggerExit(_left);
				else
					_right->OnCollisionExit(_left);

				iter->second = false;
			}
		}
	}

	bool CollisionManager::Intersect(ICollider2D* _left, ICollider2D* _right)
	{
		// Rect vs Rect 
		// 0 --- 1
		// |     |
		// 3 --- 2
		math::Vector3 arrLocalPos[4] =
		{
			math::Vector3{-0.5f, 0.5f, 0.0f}
			,math::Vector3{0.5f, 0.5f, 0.0f}
			,math::Vector3{0.5f, -0.5f, 0.0f}
			,math::Vector3{-0.5f, -0.5f, 0.0f}
		};

		Transform& leftTr = _left->GetOwner()->GetTransform();
		Transform& rightTr = _right->GetOwner()->GetTransform();

		math::Matrix leftMat = leftTr.GetWorldMatrix();
		math::Matrix rightMat = rightTr.GetWorldMatrix();



		// 분리축 벡터 4개 구하기
		math::Vector3 Axis[4] = {};

		math::Vector3 leftScale = math::Vector3(_left->GetSize().x, _left->GetSize().y, 1.0f);

		math::Matrix finalLeft = math::Matrix::CreateScale(leftScale);
		finalLeft *= leftMat;

		math::Vector3 rightScale = math::Vector3(_right->GetSize().x, _right->GetSize().y, 1.0f);
		math::Matrix finalRight = math::Matrix::CreateScale(rightScale);
		finalRight *= rightMat;

		Axis[0] = math::Vector3::Transform(arrLocalPos[1], finalLeft);
		Axis[1] = math::Vector3::Transform(arrLocalPos[3], finalLeft);
		Axis[2] = math::Vector3::Transform(arrLocalPos[1], finalRight);
		Axis[3] = math::Vector3::Transform(arrLocalPos[3], finalRight);

		Axis[0] -= math::Vector3::Transform(arrLocalPos[0], finalLeft);
		Axis[1] -= math::Vector3::Transform(arrLocalPos[0], finalLeft);
		Axis[2] -= math::Vector3::Transform(arrLocalPos[0], finalRight);
		Axis[3] -= math::Vector3::Transform(arrLocalPos[0], finalRight);

		for (int index = 0; index < 4; index++)
			Axis[index].z = 0.0f;

		math::Vector3 vc = leftTr.GetPosition() - rightTr.GetPosition();
		vc.z = 0.0f;

		math::Vector3 centerDir = vc;
		for (int index = 0; index < 4; index++)
		{
			math::Vector3 vA = Axis[index];
			//vA.Normalize();

			float projDist = 0.0f;
			for (int employee = 0; employee < 4; employee++)
			{
				projDist += fabsf(Axis[employee].Dot(vA) / 2.0f);
			}

			if (projDist < fabsf(centerDir.Dot(vA)))
			{
				return false;
			}
		}


		return true;
	}
}
