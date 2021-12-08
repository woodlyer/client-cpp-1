//
//  TDF SDK
//
//  Created by Sujan Reddy on 2020/04/17
//  Copyright 2020 Virtru Corporation
//

#ifndef VIRTRU_TDF_UTILS_H
#define VIRTRU_TDF_UTILS_H

#include <entity_object.h>
#include <string>
#include <unordered_map>

namespace virtru {
    class Utils {
      public:
        /// Get the KAS public key from the KAS well-known HTTP endpoint
        static std::string getKasPubkeyFromURL(const std::string &kasGetPublicKeyUrl);

        /// Get the entity object from eas.
        /// \param easUrl - The eas url.
        /// \param sdkConsumerCertAuthority - The cert authority file.
        /// \param headers - The HTTP headers that will used for getting the entity object.
        /// \param body - The POST for getting the entity object.
        /// \return The entity object.
        static EntityObject getEntityObject(const std::string &easUrl,
                                            const std::string &sdkConsumerCertAuthority,
                                            const std::string &clientKeyFileName,
                                            const std::string &clientCertFileName,
                                            const std::unordered_map<std::string, std::string> &headers,
                                            std::string body);

        /// Compare to two strings for equality(NOTE: only ascii)
        /// \param str1 - First string to compare.
        /// \param str2 - Second string to compare.
        /// \return Return true if both strings are lexicographically equal.
        static bool iequals(const std::string &str1, const std::string &str2);

        /// Check for the Endianness and return true if it's little-endian
        /// \return True if it's little-endian
        static bool isRunningOnLittleEndian();

        /// Encode a string to percent-encoded URL-safe format
        /// (RFC2396) - feel free to replace this with a standardized library
        /// \param str - The string to escape
        /// \return std::string - A new, RFC2396 encoded string
        static std::string urlEncode(const std::string &str);

        /// Check if the HTTP status code is ok
        /// \param status - The HTTP status code
        /// \return bool - True if the status is ok.
        static bool goodHttpStatus(const unsigned status);
    };
} // namespace virtru

#endif // VIRTRU_TDF_UTILS_H