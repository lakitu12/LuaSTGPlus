/**
 * @file
 * @date 2022/3/6
 * @author 9chu
 * 这个文件是 LuaSTGPlus 项目的一部分，请在项目所定义之授权许可范围内合规使用。
 */
#pragma once
#include <memory>
#include "SubsystemEvent.hpp"

namespace lstg::Subsystem
{
    class SubsystemContainer;
    
    /**
     * 子系统基类
     */
    class ISubsystem
    {
    public:
        ISubsystem() = default;
        virtual ~ISubsystem() noexcept = default;

    public:
        /**
         * 更新系统
         * 默认无行为。
         * @param elapsedTime 距离上次调用的流逝时间（秒）
         */
        virtual void OnUpdate(double elapsedTime) noexcept;

        /**
         * 用户渲染操作之前
         * @param elapsedTime 距离上次调用的流逝时间（秒）
         */
        virtual void OnBeforeRender(double elapsedTime) noexcept;

        /**
         * 用户渲染操作之后
         * @param elapsedTime 距离上次调用的流逝时间（秒）
         */
        virtual void OnAfterRender(double elapsedTime) noexcept;

        /**
         * 触发事件
         * 默认无行为。
         * @param event 事件
         */
        virtual void OnEvent(SubsystemEvent& event) noexcept;
    };

    using SubsystemPtr = std::shared_ptr<ISubsystem>;
}
