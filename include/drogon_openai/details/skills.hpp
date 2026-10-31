/**
 * @file skills.hpp
 * @brief skills API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/skills.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK skills.versions.content resource.
     */
    class SkillsVersionsContent {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit SkillsVersionsContent(std::shared_ptr<Transport> transport);

        /**
         * @brief GET /skills/{skill_id}/versions/{version}/content.
         * @param version Owned path identifier, encoded as one segment.
         * @param skill_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string version,
            std::string skill_id,
            types::skills::SkillsVersionsContentRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<std::string>>>;
    };

    /**
     * @brief Value handle for the SDK skills.content resource.
     */
    class SkillsContent {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit SkillsContent(std::shared_ptr<Transport> transport);

        /**
         * @brief GET /skills/{skill_id}/content.
         * @param skill_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string skill_id,
            types::skills::SkillsContentRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<std::string>>>;
    };

    /**
     * @brief Value handle for the SDK skills.versions resource.
     */
    class SkillsVersions {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit SkillsVersions(std::shared_ptr<Transport> transport);
        SkillsVersionsContent content; ///< Nested SDK resource content.

        /**
         * @brief POST /skills/{skill_id}/versions.
         * @param skill_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            std::string skill_id,
            types::skills::VersionCreateParams params = {}
        ) const -> drogon::Task<Result<types::skills::SkillVersion>>;

        /**
         * @brief POST /skills/{skill_id}/versions.
         * @param skill_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            std::string skill_id,
            types::skills::VersionCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::skills::SkillVersion>>>;

        /**
         * @brief GET /skills/{skill_id}/versions/{version}.
         * @param version Owned path identifier, encoded as one segment.
         * @param skill_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string version,
            std::string skill_id,
            types::skills::SkillsVersionsRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::skills::SkillVersion>>;

        /**
         * @brief GET /skills/{skill_id}/versions/{version}.
         * @param version Owned path identifier, encoded as one segment.
         * @param skill_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string version,
            std::string skill_id,
            types::skills::SkillsVersionsRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::skills::SkillVersion>>>;

        /**
         * @brief GET /skills/{skill_id}/versions.
         * @param skill_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            std::string skill_id,
            types::skills::VersionListParams params = {}
        ) const -> drogon::Task<Result<Page<types::skills::SkillVersion>>>;

        /**
         * @brief DELETE /skills/{skill_id}/versions/{version}.
         * @param version Owned path identifier, encoded as one segment.
         * @param skill_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Delete(
            std::string version,
            std::string skill_id,
            types::skills::SkillsVersionsDeleteParams params = {}
        ) const -> drogon::Task<Result<types::skills::DeletedSkillVersion>>;

        /**
         * @brief DELETE /skills/{skill_id}/versions/{version}.
         * @param version Owned path identifier, encoded as one segment.
         * @param skill_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto DeleteRaw(
            std::string version,
            std::string skill_id,
            types::skills::SkillsVersionsDeleteParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::skills::DeletedSkillVersion>>>;
    };

    /**
     * @brief Value handle for the SDK skills resource.
     */
    class Skills {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit Skills(std::shared_ptr<Transport> transport);
        SkillsContent content;   ///< Nested SDK resource content.
        SkillsVersions versions; ///< Nested SDK resource versions.

        /**
         * @brief POST /skills.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::skills::SkillCreateParams params = {}
        ) const -> drogon::Task<Result<types::skills::Skill>>;

        /**
         * @brief POST /skills.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::skills::SkillCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::skills::Skill>>>;

        /**
         * @brief GET /skills/{skill_id}.
         * @param skill_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string skill_id,
            types::skills::SkillsRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::skills::Skill>>;

        /**
         * @brief GET /skills/{skill_id}.
         * @param skill_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string skill_id,
            types::skills::SkillsRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::skills::Skill>>>;

        /**
         * @brief POST /skills/{skill_id}.
         * @param skill_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Update(
            std::string skill_id,
            types::skills::SkillUpdateParams params = {}
        ) const -> drogon::Task<Result<types::skills::Skill>>;

        /**
         * @brief POST /skills/{skill_id}.
         * @param skill_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto UpdateRaw(
            std::string skill_id,
            types::skills::SkillUpdateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::skills::Skill>>>;

        /**
         * @brief GET /skills.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            types::skills::SkillListParams params = {}
        ) const -> drogon::Task<Result<Page<types::skills::Skill>>>;

        /**
         * @brief DELETE /skills/{skill_id}.
         * @param skill_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Delete(
            std::string skill_id,
            types::skills::SkillsDeleteParams params = {}
        ) const -> drogon::Task<Result<types::skills::DeletedSkill>>;

        /**
         * @brief DELETE /skills/{skill_id}.
         * @param skill_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto DeleteRaw(
            std::string skill_id,
            types::skills::SkillsDeleteParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::skills::DeletedSkill>>>;
    };
} // namespace drogon_openai::details
