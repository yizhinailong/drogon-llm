/**
 * @file videos.hpp
 * @brief videos API groups generated from openai-python 3.41.0.
 */
#pragma once
#include <memory>

#include "drogon_openai/pagination.hpp"
#include "drogon_openai/types/videos.hpp"
#include "drogon_openai/websocket.hpp"

namespace drogon_openai::details {
    class Transport;

    /**
     * @brief Value handle for the SDK videos resource.
     */
    class Videos {
    private:
        std::shared_ptr<Transport> m_transport; ///< Shared immutable endpoint settings.

    public:
        /**
         * @brief Bind this resource and its children to an endpoint.
         * @param transport Shared endpoint settings.
         */
        explicit Videos(std::shared_ptr<Transport> transport);

        /**
         * @brief POST /videos.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Create(
            types::videos::VideoCreateParams params = {}
        ) const -> drogon::Task<Result<types::videos::Video>>;

        /**
         * @brief POST /videos.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateRaw(
            types::videos::VideoCreateParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::videos::Video>>>;

        /**
         * @brief GET /videos/{video_id}.
         * @param video_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Retrieve(
            std::string video_id,
            types::videos::VideosRetrieveParams params = {}
        ) const -> drogon::Task<Result<types::videos::Video>>;

        /**
         * @brief GET /videos/{video_id}.
         * @param video_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RetrieveRaw(
            std::string video_id,
            types::videos::VideosRetrieveParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::videos::Video>>>;

        /**
         * @brief GET /videos.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto List(
            types::videos::VideoListParams params = {}
        ) const -> drogon::Task<Result<Page<types::videos::Video>>>;

        /**
         * @brief DELETE /videos/{video_id}.
         * @param video_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Delete(
            std::string video_id,
            types::videos::VideosDeleteParams params = {}
        ) const -> drogon::Task<Result<types::videos::VideoDeleteResponse>>;

        /**
         * @brief DELETE /videos/{video_id}.
         * @param video_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto DeleteRaw(
            std::string video_id,
            types::videos::VideosDeleteParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::videos::VideoDeleteResponse>>>;

        /**
         * @brief POST /videos/characters.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateCharacter(
            types::videos::VideoCreateCharacterParams params = {}
        ) const -> drogon::Task<Result<types::videos::VideoCreateCharacterResponse>>;

        /**
         * @brief POST /videos/characters.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto CreateCharacterRaw(
            types::videos::VideoCreateCharacterParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::videos::VideoCreateCharacterResponse>>>;

        /**
         * @brief GET /videos/{video_id}/content.
         * @param video_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto DownloadContent(
            std::string video_id,
            types::videos::VideoDownloadContentParams params = {}
        ) const -> drogon::Task<Result<RawResponse<std::string>>>;

        /**
         * @brief POST /videos/edits.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Edit(
            types::videos::VideoEditParams params = {}
        ) const -> drogon::Task<Result<types::videos::Video>>;

        /**
         * @brief POST /videos/edits.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto EditRaw(
            types::videos::VideoEditParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::videos::Video>>>;

        /**
         * @brief POST /videos/extensions.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Extend(
            types::videos::VideoExtendParams params = {}
        ) const -> drogon::Task<Result<types::videos::Video>>;

        /**
         * @brief POST /videos/extensions.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto ExtendRaw(
            types::videos::VideoExtendParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::videos::Video>>>;

        /**
         * @brief GET /videos/characters/{character_id}.
         * @param character_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto GetCharacter(
            std::string character_id,
            types::videos::VideosGetCharacterParams params = {}
        ) const -> drogon::Task<Result<types::videos::VideoGetCharacterResponse>>;

        /**
         * @brief GET /videos/characters/{character_id}.
         * @param character_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto GetCharacterRaw(
            std::string character_id,
            types::videos::VideosGetCharacterParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::videos::VideoGetCharacterResponse>>>;

        /**
         * @brief POST /videos/{video_id}/remix.
         * @param video_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto Remix(
            std::string video_id,
            types::videos::VideoRemixParams params = {}
        ) const -> drogon::Task<Result<types::videos::Video>>;

        /**
         * @brief POST /videos/{video_id}/remix.
         * @param video_id Owned path identifier, encoded as one segment.
         * @param params Owned SDK parameters and request overrides.
         * @return API result or request/validation error.
         */
        [[nodiscard]] auto RemixRaw(
            std::string video_id,
            types::videos::VideoRemixParams params = {}
        ) const -> drogon::Task<Result<RawResponse<types::videos::Video>>>;

        /**
         * @brief Poll a video until completed or failed.
         * @param video_id Owned operation parameters.
         * @param poll Owned operation parameters.
         * @param options Owned operation parameters.
         * @return Operation result or an error.
         */
        [[nodiscard]] auto Poll(
            std::string video_id,
            PollOptions poll = {},
            RequestOptions options = {}
        ) const -> drogon::Task<Result<types::videos::Video>>;

        /**
         * @brief Create a video and poll the resulting resource.
         * @param params Owned operation parameters.
         * @param poll Owned operation parameters.
         * @return Operation result or an error.
         */
        [[nodiscard]] auto CreateAndPoll(
            types::videos::VideoCreateParams params,
            PollOptions poll = {}
        ) const -> drogon::Task<Result<types::videos::Video>>;
    };
} // namespace drogon_openai::details
