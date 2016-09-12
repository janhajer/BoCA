#pragma once
#include "boca/generic/EnumIterator.hh"
namespace boca
{

/**
 * @ingroup Generic
 * @defgroup Iterator
 * @brief Iterators
 * @{
 */

/**
* @brief %Iterator
*/
template <template <typename> class Container_, template <typename> class SubContainer_, typename Value_, typename Enum_>
class Iterator2
{
public:
    Iterator2(Container_<Value_>* container, Enum_ position) :
        position_(position),
        container_(container)
    {}

    bool operator!= (Iterator2 const& iterator) const {
        return position_ != iterator.position_;
    }

    SubContainer_<Value_>& operator*() {
        return (*container_)(*position_);
    }

    Iterator2 const& operator++ () {
        ++position_;
        return *this;
    }

private:
    EnumIterator<Enum_> position_;
    Container_<Value_>* container_;
};

/**
* @brief const iterator
*/
template <template <typename> class Container_, template <typename> class SubContainer_, typename Value_, typename Enum_>
class ConstIterator2
{
public:
    ConstIterator2(Container_<Value_> const* container, Enum_ position) :
        position_(position),
        container_(container)
    {}

    bool operator!= (ConstIterator2 const& iterator) const {
        return position_ != iterator.position_;
    }

    SubContainer_<Value_> operator*() const {
        return (*container_)(*position_);
    }

    ConstIterator2 const& operator++ () {
        ++position_;
        return *this;
    }

private:
    EnumIterator<Enum_> position_;
    Container_<Value_> const* container_;
};

/**
* @brief %Iterator
*/
template <template <typename> class Container_, typename Value_, typename Enum_>
class Iterator
{
public:
    Iterator(Container_<Value_>* container, Enum_ position) :
        position_(position),
        container_(container)
    {}

    bool operator!= (Iterator& iterator) const {
        return position_ != iterator.position_;
    }

    Value_& operator*() {
        return (*container_)(*position_);
    }

    Iterator const& operator++ () {
        ++position_;
        return *this;
    }

private:
    EnumIterator<Enum_> position_;
    Container_<Value_>* container_;
};

/**
* @brief const iterator
*/
template <template <typename> class Container_, typename Value_, typename Enum_>
class ConstIterator
{
public:
    ConstIterator(Container_<Value_> const* container, Enum_ position) :
        position_(position),
        container_(container)
    {}

    bool operator!= (ConstIterator const& iterator) const {
        return position_ != iterator.position_;
    }

    Value_ operator*() const {
        return (*container_)(*position_);
    }

    ConstIterator const& operator++ () {
        ++position_;
        return *this;
    }

private:
    EnumIterator<Enum_> position_;
    Container_<Value_> const* container_;
};

}

/*
 * @}
 */
