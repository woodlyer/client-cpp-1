/*
* Copyright 2019 Virtru Corporation
*
* SPDX - License Identifier: BSD-3-Clause-Clear
*
*/
//
//  TDF SDK
//
//  Created by Sujan Reddy on 2019/04/03.
//

#ifndef VIRTRU_SPLITKEY_ENCRYPTION_H
#define VIRTRU_SPLITKEY_ENCRYPTION_H

#include "encryption_strategy.h"
#include "tdf_constants.h"
#include "crypto/crypto_utils.h"
#include "manifest_data_model.h"

#include <memory>

namespace virtru {

    using namespace virtru::crypto;

    class SplitKey : public EncryptionStrategy {
    public: /// Interface

        /// Constructor
        explicit SplitKey(CipherType chiperType);

        /// Destructor
        ~SplitKey() override;

        // Not supported.
        SplitKey() = delete;
        SplitKey(const SplitKey &) = delete;
        SplitKey(SplitKey &&) = delete;
        SplitKey & operator=(const SplitKey &) = delete;
        SplitKey & operator=(SplitKey &&) = delete;

    public: // EncryptionStrategy

        /// Add KeyAccess object.
        /// \param KeyAccess - A keyAccess object.
        void addKeyAccess(std::unique_ptr<KeyAccess> keyAccess) override;

        /// Generate and return manifest.
        /// \return - The manifest representation of data model.
        ManifestDataModel getManifest() override;

        /// Encrypt the data using the cipher.
        /// \param data - A buffer which contains data to be encrypted
        /// \param encryptedData - A buffer for encrypted data output
        void encrypt(Bytes data, WriteableBytes& encryptedData) const override;
        
        /// Decrypt the data using the cipher.
        /// \param data - A buffer which contains data to be decrypted
        /// \param decryptedData - A buffer for decrypted data output
        void decrypt(Bytes data, WriteableBytes& decryptedData) const override;

        /// Return the wrapped key used by split key encryption
        /// \return - The wrapped key.
        WrappedKey& getPayloadKey() {
            if (m_payloadKeyOverride) {
                return m_payloadKey;
            }
            return m_key;
        }

        /// Set the wrapped key that will be used by split key encryption
        /// \param key - The wrapped key.
        void setWrappedKey(const WrappedKey& key);

        /// Set the payload key that will be used by split key encryption
        /// \param key - The payload key.
        void setPayloadKey(const WrappedKey& key);

        /// Return the policy key used by split key encryption
        /// \return - The policy key.
        WrappedKey getPolicyKey() { return m_key;}

    private:
        /// Encrypt the data using the cipher.
        /// \param data - A buffer which contains data to be encrypted
        /// \param encryptedData - A buffer for encrypted data output
        void encrypt(Bytes iv, Bytes data, WriteableBytes& encryptedData, bool isMetaData = false) const;
        
    private: // Data
        CipherType m_cipherType;
        WrappedKey m_key = symmetricKey<kKeyLength>();
        WrappedKey m_payloadKey;
        bool m_payloadKeyOverride{false};
        std::vector<std::unique_ptr<KeyAccess>> m_keyAccessObjects;

        // TODO: May want to have 2MB buffer which can be reused for encryption/decryption.
    };
}  // namespace virtru

#endif //VIRTRU_SPLITKEY_ENCRYPTION_H

