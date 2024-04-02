///
/// @file LockableView.hpp
/// @author Andréas Leroux (andreas.leroux@epitech.eu)
/// @brief
/// @version 1.0.0
/// @date 2024-04-02
///
/// @copyright Copyright (c) ECSTASY 2022 - 2024
///
///

#ifndef ECSTASY_THREAD_LOCKABLEVIEW_HPP_
#define ECSTASY_THREAD_LOCKABLEVIEW_HPP_

#include "ecstasy/thread/Lockable.hpp"

namespace ecstasy::thread
{
    ///
    /// @brief Non template base class for LockableView<T> types.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-03)
    ///
    class LockableViewBase {
      public:
        virtual ~LockableViewBase() = default;
    };

    ///
    /// @brief Thread safe view of a Lockable object.
    /// This view lock the object on construction and unlock it on destruction.
    ///
    /// @note If L is of type const SharedRecursiveMutex, the view will be read only and the lock will be shared.
    /// @note It can be seen as a thread safe reference wrapper.
    ///
    /// @tparam L Type of the Lockable object.
    ///
    /// @author Andréas Leroux (andreas.leroux@epitech.eu)
    /// @since 1.0.0 (2024-04-03)
    ///
    template <Lockable L>
    class LockableView : public LockableViewBase {
      public:
        /// Type of the Lockable object.
        using LockableType = L;

        ///
        /// @brief Construct a new Lockable View.
        ///
        /// @param[in] lockable
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-03)
        ///
        LockableView(L &lockable) : _lockable(lockable)
        {
            _lockable.lock();
        }

        ///
        /// @brief Copy constructor.
        ///
        /// @note This relock the object.
        ///
        /// @param[in] other Other Lockable View to copy.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-03)
        ///
        LockableView(const LockableView<L> &other) noexcept : _lockable(other._lockable)
        {
            _lockable.lock();
        }

        ///
        /// @brief Move constructor.
        ///
        /// @warning This doesn't really take the lock from @p other, it just lock the object again. This is because we
        /// can't unset the reference of @p other and it will unlock the object on destruction.
        /// @warning This act as the copy constructor.
        ///
        /// @param[in] other Other Lockable View to move.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-03)
        ///
        LockableView(const LockableView<L> &&other) noexcept : _lockable(other._lockable)
        {
            _lockable.lock();
        }

        ///
        /// @brief Destroy the view, unlocking the internal lockable.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-03)
        ///
        ~LockableView()
        {
            _lockable.unlock();
        }

        ///
        /// @brief Access the internal lockable object.
        ///
        /// @return L& Reference to the internal lockable object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-03)
        ///
        L &operator*()
        {
            return _lockable;
        }

        ///
        /// @brief Access the internal lockable object pointer.
        ///
        /// @return L* Pointer to the internal lockable object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-03)
        ///
        L *operator->()
        {
            return &_lockable;
        }

        ///
        /// @brief Access the internal lockable object pointer.
        ///
        /// @warning This method is const but the object is not. The view does not change the constness of the object,
        /// for ensured const access use LockableView<const L>.
        ///
        /// @return L* Pointer to the internal lockable object.
        ///
        /// @author Andréas Leroux (andreas.leroux@epitech.eu)
        /// @since 1.0.0 (2024-04-03)
        ///
        L *operator->() const
        {
            return &_lockable;
        }

      private:
        L &_lockable;
    };

} // namespace ecstasy::thread

#endif /* !ECSTASY_THREAD_LOCKABLEVIEW_HPP_ */
