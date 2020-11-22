//#include <FEngine/ResourcesManager/PostEffectsSerializer.h>
//
//#include <FEngine/Utils/SerializableObjectParser.h>
//
//namespace fengine
//{
//    const char *PostEffectSerializer::k_postEffects = "../Resources/PostEffects.txt";
//    const char *PostEffectSerializer::k_countTag = "Count ";
//
//    PostEffectPassBehaviour * PostEffectSerializer::GetPostEffectPassBehaviour(obj_id id)
//    {
//        	FILE *file = fopen(k_postEffects, "r");
//        
//        	if (file)
//        	{
//        		unsigned int postEffectsCount;
//        		fscanf_s(file, k_countTag);
//        		fscanf_s(file, "%u ", &postEffectsCount);
//        
//        		for (unsigned i = 0; i < postEffectsCount; ++i)
//        		{
//        			fengine::SerializableObject *object = fengine::SerializableObjectParser::ReadResource(file);
//        			fengine::PostEffectPassBehaviour *passBehaviour = dynamic_cast<fengine::PostEffectPassBehaviour*>(object);
//        			obj_id objectId = passBehaviour->GetId();
//        			if (objectId == id)
//        			{
//        				return passBehaviour;
//        			}
//        		}
//        
//        		fclose(file);
//        	}
//
//        return nullptr;
//    }
//}
