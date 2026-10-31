/**
 * @file headers.hpp
 * @brief Own HTTP header fields with independent copies and non-throwing moves.
 */
#pragma once

#include <initializer_list>
#include <map>
#include <memory>
#include <string>
#include <utility>

namespace drogon_openai {

    /**
     * @brief Ordered HTTP header fields; request processing matches names without case.
     * @note Indirect ownership keeps moves non-throwing even when the standard map allocates on move.
     */
    class Headers {
    public:
        using Storage = std::map<std::string, std::string>; ///< Underlying ordered fields.
        using key_type = Storage::key_type;                 ///< Header name.
        using mapped_type = Storage::mapped_type;           ///< Header value.
        using value_type = Storage::value_type;             ///< Name/value pair.
        using size_type = Storage::size_type;               ///< Field count.
        using iterator = Storage::iterator;                 ///< Mutable field iterator.
        using const_iterator = Storage::const_iterator;     ///< Read-only field iterator.

    private:
        mutable std::unique_ptr<Storage> m_fields; ///< Null represents moved-from storage.

        /**
         * @brief Materialize mutable storage on first use.
         * @return Owned fields.
         */
        auto fields() -> Storage& {
            if (!m_fields) {
                m_fields = std::make_unique<Storage>();
            }
            return *m_fields;
        }

        /**
         * @brief Read fields, restoring empty storage after a move when needed.
         * @return Read-only fields.
         */
        auto fields() const -> Storage const& {
            if (!m_fields) {
                m_fields = std::make_unique<Storage>();
            }
            return *m_fields;
        }

    public:
        /**
         * @brief Construct empty header fields.
         */
        Headers() : m_fields{ std::make_unique<Storage>() } {}

        /**
         * @brief Initialize header fields from name/value pairs.
         * @param values Initial fields.
         */
        Headers(std::initializer_list<value_type> values)
            : m_fields{ std::make_unique<Storage>(values) } {}

        /**
         * @brief Import an existing standard map.
         * @param values Initial fields, taken by value.
         */
        Headers(Storage values)
            : m_fields{ std::make_unique<Storage>(std::move(values)) } {}

        /**
         * @brief Copy fields into independent storage.
         * @param other Fields to copy.
         */
        Headers(Headers const& other)
            : m_fields{ other.m_fields ? std::make_unique<Storage>(*other.m_fields) : nullptr } {}

        /**
         * @brief Transfer ownership without moving or allocating a map.
         * @param other Fields to transfer; left empty.
         */
        Headers(Headers&& other) noexcept = default;

        /**
         * @brief Replace fields with an independent copy.
         * @param other Fields to copy.
         * @return This container.
         */
        auto operator=(Headers const& other) -> Headers& {
            if (this != &other) {
                Headers copy{ other };
                swap(copy);
            }
            return *this;
        }

        /**
         * @brief Transfer ownership without moving or allocating a map.
         * @param other Fields to transfer; left empty.
         * @return This container.
         */
        auto operator=(Headers&& other) noexcept -> Headers& = default;

        /**
         * @brief Replace fields with name/value pairs.
         * @param values Replacement fields.
         * @return This container.
         */
        auto operator=(std::initializer_list<value_type> values) -> Headers& {
            Headers replacement{ values };
            swap(replacement);
            return *this;
        }

        /**
         * @brief Access a field, inserting an empty value when absent.
         * @param name Header name.
         * @return Mutable header value.
         */
        auto operator[](std::string name) -> std::string& { return fields()[std::move(name)]; }

        /**
         * @brief Access an existing field.
         * @param name Header name.
         * @return Mutable header value.
         * @throws std::out_of_range The field is absent.
         */
        auto at(std::string const& name) -> std::string& { return fields().at(name); }

        /**
         * @brief Access an existing field.
         * @param name Header name.
         * @return Read-only header value.
         * @throws std::out_of_range The field is absent.
         */
        auto at(std::string const& name) const -> std::string const& { return fields().at(name); }

        /**
         * @brief Find a field by its stored name.
         * @param name Header name.
         * @return Matching iterator or end().
         */
        auto find(std::string const& name) -> iterator { return fields().find(name); }

        /**
         * @brief Find a field by its stored name.
         * @param name Header name.
         * @return Matching iterator or end().
         */
        auto find(std::string const& name) const -> const_iterator { return fields().find(name); }

        /**
         * @brief Test whether a stored name is present.
         * @param name Header name.
         * @return Whether the field exists.
         */
        auto contains(std::string const& name) const -> bool { return fields().contains(name); }

        /**
         * @brief Count fields with a stored name.
         * @param name Header name.
         * @return Zero or one.
         */
        auto count(std::string const& name) const -> size_type { return fields().count(name); }

        /**
         * @brief Insert a field unless its stored name already exists.
         * @tparam Args Field construction argument types.
         * @param args Field construction arguments.
         * @return Field iterator and whether insertion occurred.
         */
        template <typename... Args>
        auto emplace(Args&&... args) -> std::pair<iterator, bool> {
            return fields().emplace(std::forward<Args>(args)...);
        }

        /**
         * @brief Insert a field unless its stored name already exists.
         * @param value Field to insert.
         * @return Field iterator and whether insertion occurred.
         */
        auto insert(value_type value) -> std::pair<iterator, bool> { return fields().insert(std::move(value)); }

        /**
         * @brief Erase a field by its stored name.
         * @param name Header name.
         * @return Number of fields removed.
         */
        auto erase(std::string const& name) -> size_type { return m_fields ? m_fields->erase(name) : 0; }

        /**
         * @brief Erase a field at an iterator.
         * @param position Field to remove.
         * @return Iterator following the removed field.
         */
        auto erase(const_iterator position) -> iterator { return fields().erase(position); }

        /**
         * @brief Remove all fields while preserving iterator ownership.
         */
        auto clear() noexcept -> void {
            if (m_fields) {
                m_fields->clear();
            }
        }

        /**
         * @brief Test whether there are no fields.
         * @return Whether the container is empty.
         */
        auto empty() const noexcept -> bool { return !m_fields || m_fields->empty(); }

        /**
         * @brief Count the stored fields.
         * @return Field count.
         */
        auto size() const noexcept -> size_type { return m_fields ? m_fields->size() : 0; }

        /**
         * @brief Start mutable iteration in name order.
         * @return First field iterator.
         */
        auto begin() -> iterator { return fields().begin(); }

        /**
         * @brief Finish mutable iteration in name order.
         * @return Past-the-end iterator.
         */
        auto end() -> iterator { return fields().end(); }

        /**
         * @brief Start read-only iteration in name order.
         * @return First field iterator.
         */
        auto begin() const -> const_iterator { return fields().begin(); }

        /**
         * @brief Finish read-only iteration in name order.
         * @return Past-the-end iterator.
         */
        auto end() const -> const_iterator { return fields().end(); }

        /**
         * @brief Start read-only iteration in name order.
         * @return First field iterator.
         */
        auto cbegin() const -> const_iterator { return begin(); }

        /**
         * @brief Finish read-only iteration in name order.
         * @return Past-the-end iterator.
         */
        auto cend() const -> const_iterator { return end(); }

        /**
         * @brief Exchange field ownership without allocating.
         * @param other Container to exchange with.
         */
        auto swap(Headers& other) noexcept -> void { m_fields.swap(other.m_fields); }

        /**
         * @brief Compare the stored name/value pairs.
         * @param other Fields to compare with.
         * @return Whether both containers hold the same fields.
         */
        auto operator==(Headers const& other) const -> bool { return fields() == other.fields(); }
    };

} // namespace drogon_openai
