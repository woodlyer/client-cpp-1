/*
* Copyright 2019 Virtru Corporation
*
* SPDX - License Identifier: BSD-3-Clause-Clear
*
*/
//
//  TDF SDK
//
//  Created by Elizabeth Healy on 2022/04/25.
//

//TODO these should be grouped more logically, by kind of failure,
//in terms of what the *caller* is interested in, and correctly propagated back up thru the
//SDK API.
//
//e.g.
//- is this a Permission (key release) failure?
//- is this a System (any of the following) failure?
//  - is this a TDF format failure?
//  - is this an "I can't talk to KAS" failure?
//  - is this an "I can't authenticate via OIDC" failure?
//etc etc
namespace virtru {

    /// error codes
    #define VIRTRU_GENERAL_ERROR 1 /* General error code, non-specific */
    #define VIRTRU_SYSTEM_ERROR 500 /* System error: ex. read write/source sink errors */
    #define VIRTRU_NETWORK_ERROR 1000 /* Netowrk error */
    #define VIRTRU_CRYPTO_ERROR 2000 /* Crypto error */
    #define VIRTRU_TDF_FORMAT_ERROR 3000 /* TDF format error */
    #define VIRTRU_ATTR_OBJ_ERROR 3100 /* Attribute object error */
    #define VIRTRU_POLICY_OBJ_ERROR 3200 /* Policy object error */
    #define VIRTRU_KAS_OBJ_ERROR 3300 /* Key access object error */
    #define VIRTRU_ENTITY_OBJ_ERROR 3400 /* Entity object error */
    #define VIRTRU_NANO_TDF_FORMAT_ERROR 4000 /* Nano TDF format error */

}
