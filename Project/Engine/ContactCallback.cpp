#include "PCH_Engine.h"
#include "SimpleMath.h"
#include "ContactCallback.h"

#include "GameObject.h"
#include "ICollider.h"

namespace mh
{
    void ContactCallback::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pPairs, PxU32 iNbPairs)
    {
        for (PxU32 i = 0; i < iNbPairs; i++)
        {
            const PxContactPair& cp = pPairs[i];

            if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
            {
                if (pairHeader.actors[0]->userData && pairHeader.actors[1]->userData)
                {
                    GameObject* pContact = static_cast<GameObject*>(pairHeader.actors[0]->userData);
                    GameObject* pOther = static_cast<GameObject*>(pairHeader.actors[1]->userData);

                    if (nullptr != pContact && nullptr != pOther)
                    {
                        if (pContact->GetComponent<ICollider3D>())
                            pContact->GetComponent<ICollider3D>()->OnCollisionEnter(pOther->GetComponent<ICollider3D>());

                        if (pOther->GetComponent<ICollider3D>())
                            pOther->GetComponent<ICollider3D>()->OnCollisionEnter(pContact->GetComponent<ICollider3D>());
                    }
                }
            }

            else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
            {
                if (pairHeader.actors[0]->userData && pairHeader.actors[1]->userData)
                {
                    GameObject* pContact = static_cast<GameObject*>(pairHeader.actors[0]->userData);
                    GameObject* pOther = static_cast<GameObject*>(pairHeader.actors[1]->userData);

                    if (nullptr != pContact && nullptr != pOther)
                    {
                        if (pContact->GetComponent<ICollider3D>())
                            pContact->GetComponent<ICollider3D>()->OnCollisionStay(pOther->GetComponent<ICollider3D>());

                        if (pOther->GetComponent<ICollider>())
                            pOther->GetComponent<ICollider3D>()->OnCollisionStay(pContact->GetComponent<ICollider3D>());
                    }
                }
            }

            else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
            {
                if (pairHeader.actors[0]->userData && pairHeader.actors[1]->userData)
                {
                    GameObject* pContact = static_cast<GameObject*>(pairHeader.actors[0]->userData);
                    GameObject* pOther = static_cast<GameObject*>(pairHeader.actors[1]->userData);

                    if (nullptr != pContact && nullptr != pOther)
                    {
                        if (pContact->GetComponent<ICollider3D>())
                            pContact->GetComponent<ICollider3D>()->OnCollisionExit(pOther->GetComponent<ICollider3D>());

                        if (pOther->GetComponent<ICollider3D>())
                            pOther->GetComponent<ICollider3D>()->OnCollisionExit(pContact->GetComponent<ICollider3D>());
                    }
                }
            }
        }
    }

    void ContactCallback::onTrigger(PxTriggerPair* pairs, PxU32 count)
    {
        for (PxU32 i = 0; i < count; i++)
        {
            const PxTriggerPair& pair = pairs[i];

            if (pair.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
            {
                if (pair.triggerActor && pair.otherActor)
                {
                    GameObject* pTrigger = static_cast<GameObject*>(pair.triggerActor->userData);
                    GameObject* pOther = static_cast<GameObject*>(pair.otherActor->userData);

                    if (nullptr != pTrigger && nullptr != pOther)
                    {
                        if (pTrigger->GetComponent<ICollider3D>())
                            pTrigger->GetComponent<ICollider3D>()->OnTriggerEnter(pOther->GetComponent<ICollider3D>());

                        if (pOther->GetComponent<ICollider3D>())
                            pOther->GetComponent<ICollider3D>()->OnTriggerEnter(pTrigger->GetComponent<ICollider3D>());
                    }
                }
            }

            if (pair.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
            {
                if (pair.triggerActor && pair.otherActor)
                {
                    GameObject* pTrigger = static_cast<GameObject*>(pair.triggerActor->userData);
                    GameObject* pOther = static_cast<GameObject*>(pair.otherActor->userData);

                    if (nullptr != pTrigger && nullptr != pOther)
                    {
                        if (pTrigger->GetComponent<ICollider3D>())
                            pTrigger->GetComponent<ICollider3D>()->OnTriggerExit(pOther->GetComponent<ICollider3D>());

                        if (pOther->GetComponent<ICollider3D>())
                            pOther->GetComponent<ICollider3D>()->OnTriggerExit(pTrigger->GetComponent<ICollider3D>());
                    }

                }
            }
        }
    }
}
