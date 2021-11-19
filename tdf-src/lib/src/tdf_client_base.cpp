//
//  TDF SDK
//
//  Created by Sujan Reddy on 2020/08/13
//  Copyright 2020 Virtru Corporation
//

#include "tdf_client_base.h"
#include "logger.h"
#include "tdf_exception.h"

#include <openssl/crypto.h>

namespace virtru {

    /// Constructor
    TDFClientBase::TDFClientBase(const std::string &easUrl, const std::string &user)
        : TDFClientBase(easUrl, user, "", "", "") {
    }

    /// Constructor
    TDFClientBase::TDFClientBase(const std::string &easUrl, const std::string &user,
                         const std::string &clientKeyFileName, const std::string &clientCertFileName,
                         const std::string &sdkConsumerCertAuthority)
        : m_easUrl{easUrl}, m_user{user}, m_clientKeyFileName{clientKeyFileName},
          m_clientCertFileName{clientCertFileName}, m_certAuthority{sdkConsumerCertAuthority} {
    }

    /// Destructor
    TDFClientBase::~TDFClientBase() = default;

    /// Add access to the TDF file/data for the users in the list
    void TDFClientBase::shareWithUsers(const std::vector<std::string> &users) {

        for (const auto &user : users) {
            m_dissems.insert(user);
        }
    }

    /// Enable the internal logger class to write logs to the console for given LogLevel.
    void TDFClientBase::enableConsoleLogging(LogLevel logLevel) {
        m_logLevel = logLevel;
    }

    ///Read entity attributes
    std::vector<std::string> TDFClientBase::getEntityAttributes() {
        std::vector<std::string> entityAttributes;

        auto attributeObjects = getEntityAttrObjects();
        for (const auto &attributeObj : attributeObjects) {
            entityAttributes.push_back(attributeObj.getAttribute());
        }
        return entityAttributes;
    }

    std::vector<std::string> TDFClientBase::getSubjectAttributes() {

        auto attributeObjects = getSubjectAttrObjects();

        std::vector<std::string> subjectAttributes;
        subjectAttributes.reserve(attributeObjects.size());
        for (const auto &attributeObj : attributeObjects) {
            subjectAttributes.push_back(attributeObj.getAttribute());
        }
        return subjectAttributes;
    }

    ///Add data attribute
    void TDFClientBase::withDataAttribute(const std::string &dataAttribute, const std::string &displayName, const std::string &kasPubKey, const std::string &kasURL) {
        m_dataAttributeObjects.emplace_back(dataAttribute, displayName, kasPubKey, kasURL);
    }

    ///Read data attributes associated with client
    std::vector<std::string> TDFClientBase::getDataAttributes() {
        std::vector<std::string> dataAttributes;
        //get uri's for each attribute
        for (const auto &attributeObj : m_dataAttributeObjects) {
            dataAttributes.push_back(attributeObj.getAttribute());
        }
        return dataAttributes;
    }

    /// Create the policy object.
    PolicyObject TDFClientBase::createPolicyObject() {

        auto policyObject = PolicyObject{};
        for (const auto &user : m_dissems) {
            policyObject.addDissem(user);
        }

        for (const auto &attr : m_dataAttributeObjects) {
            policyObject.addAttributeObject(attr);
        }

        return policyObject;
    }

    /// Find a default attribute in a vector of attributes
    AttributeObject TDFClientBase::getDefaultAttributeObject(const std::vector<AttributeObject> &attributeObjects) {
        //look at each attribute in vector, check if default, return
        for (const auto &attributeObj : attributeObjects) {
            if (attributeObj.isDefault()) {
                return attributeObj;
            }
        }

        if (attributeObjects.empty()) {
            ThrowException("Attribute objects does not exist");
        }

        return attributeObjects.front();
    }
} // namespace virtru
