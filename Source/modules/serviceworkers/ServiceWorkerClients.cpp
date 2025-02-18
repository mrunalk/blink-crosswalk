// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "config.h"
#include "modules/serviceworkers/ServiceWorkerClients.h"

#include "bindings/core/v8/CallbackPromiseAdapter.h"
#include "bindings/core/v8/ScriptPromiseResolver.h"
#include "core/dom/ExceptionCode.h"
#include "modules/serviceworkers/ServiceWorkerError.h"
#include "modules/serviceworkers/ServiceWorkerGlobalScopeClient.h"
#include "modules/serviceworkers/ServiceWorkerWindowClient.h"
#include "public/platform/WebServiceWorkerClientsInfo.h"
#include "wtf/RefPtr.h"
#include "wtf/Vector.h"

namespace blink {

namespace {

    class ClientArray {
    public:
        typedef blink::WebServiceWorkerClientsInfo WebType;
        static HeapVector<Member<ServiceWorkerClient> > take(ScriptPromiseResolver*, WebType* webClientsRaw)
        {
            OwnPtr<WebType> webClients = adoptPtr(webClientsRaw);
            HeapVector<Member<ServiceWorkerClient> > clients;
            for (size_t i = 0; i < webClients->clients.size(); ++i) {
                clients.append(ServiceWorkerWindowClient::create(webClients->clients[i]));
            }
            return clients;
        }
        static void dispose(WebType* webClientsRaw)
        {
            delete webClientsRaw;
        }

    private:
        WTF_MAKE_NONCOPYABLE(ClientArray);
        ClientArray() = delete;
    };

} // namespace

ServiceWorkerClients* ServiceWorkerClients::create()
{
    return new ServiceWorkerClients();
}

ServiceWorkerClients::ServiceWorkerClients()
{
}

ScriptPromise ServiceWorkerClients::getAll(ScriptState* scriptState, const ClientQueryOptions& options)
{
    RefPtrWillBeRawPtr<ScriptPromiseResolver> resolver = ScriptPromiseResolver::create(scriptState);
    ScriptPromise promise = resolver->promise();

    if (options.includeUncontrolled()) {
        // FIXME: Currently we don't support includeUncontrolled=true.
        resolver->reject(DOMException::create(NotSupportedError, "includeUncontrolled parameter of getAll is not supported."));
        return promise;
    }

    if (options.type() != "window") {
        // FIXME: Currently we only support WindowClients.
        resolver->reject(DOMException::create(NotSupportedError, "type parameter of getAll is not supported."));
        return promise;
    }

    ServiceWorkerGlobalScopeClient::from(scriptState->executionContext())->getClients(new CallbackPromiseAdapter<ClientArray, ServiceWorkerError>(resolver));
    return promise;
}

} // namespace blink
