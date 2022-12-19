#pragma once

#include <core/math/transform.hpp>

template <typename ContentStruct>
class UniformBuffer{
private:
    UniquePtr<Buffer> m_UniformBuffer;
    UniquePtr<Buffer> m_StagingBuffer;
    ContentStruct *m_StagingDataPtr{m_StagingBuffer->Map<ContentStruct>()};
public:
    UniformBuffer():
        m_UniformBuffer(
            Buffer::Create(
                sizeof(ContentStruct), 
                BufferMemoryType::DynamicVRAM, 
                BufferUsageBits::UniformBuffer | BufferUsageBits::TransferDestination
            )
        ),
        m_StagingBuffer(
            Buffer::Create(
                sizeof(ContentStruct), 
                BufferMemoryType::UncachedRAM, 
                BufferUsageBits::TransferSource
            )
        )
    {}

    void CmdUpdate(CommandBuffer& cmd_buffer, const ContentStruct& content)const{
        *m_StagingDataPtr = content;
        cmd_buffer.Copy(m_StagingBuffer.Get(), m_UniformBuffer.Get(), sizeof(ContentStruct));
    }

    void Update(const ContentStruct& content){
        *m_UniformBuffer->Map<ContentStruct>() = content;
        m_UniformBuffer->Unmap();
    }
    
    operator const Buffer*()const{
        return m_UniformBuffer.Get();
    }
};

