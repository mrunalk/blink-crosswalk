/*
 * Copyright (C) Research In Motion Limited 2011-2012. All rights reserved.
 * Copyright (C) 2013 Samsung Electronics. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef SVGAnimatedTypeAnimator_h
#define SVGAnimatedTypeAnimator_h

#include "core/svg/properties/SVGPropertyInfo.h"
#include "platform/heap/Handle.h"
#include "wtf/PassOwnPtr.h"
#include "wtf/RefPtr.h"
#include "wtf/Vector.h"
#include "wtf/text/WTFString.h"

namespace blink {

class SVGAnimatedPropertyBase;
class SVGPropertyBase;
class SVGElement;
class SVGAnimationElement;

class SVGAnimatedTypeAnimator final : public NoBaseWillBeGarbageCollectedFinalized<SVGAnimatedTypeAnimator> {
    WTF_MAKE_FAST_ALLOCATED_WILL_BE_REMOVED;
public:
    static PassOwnPtrWillBeRawPtr<SVGAnimatedTypeAnimator> create(SVGAnimationElement* animationElement, SVGElement* targetElement)
    {
        return adoptPtrWillBeNoop(new SVGAnimatedTypeAnimator(animationElement, targetElement));
    }
    ~SVGAnimatedTypeAnimator();

    PassRefPtrWillBeRawPtr<SVGPropertyBase> constructFromString(const String&);

    PassRefPtrWillBeRawPtr<SVGPropertyBase> startAnimValAnimation(const WillBeHeapVector<RawPtrWillBeMember<SVGElement> >&);
    void stopAnimValAnimation(const WillBeHeapVector<RawPtrWillBeMember<SVGElement> >&);
    PassRefPtrWillBeRawPtr<SVGPropertyBase> resetAnimValToBaseVal(const WillBeHeapVector<RawPtrWillBeMember<SVGElement> >&);

    void calculateAnimatedValue(float percentage, unsigned repeatCount, SVGPropertyBase*, SVGPropertyBase*, SVGPropertyBase*, SVGPropertyBase*);
    float calculateDistance(const String& fromString, const String& toString);

    void calculateFromAndToValues(RefPtrWillBeMember<SVGPropertyBase>& from, RefPtrWillBeMember<SVGPropertyBase>& to, const String& fromString, const String& toString);
    void calculateFromAndByValues(RefPtrWillBeMember<SVGPropertyBase>& from, RefPtrWillBeMember<SVGPropertyBase>& to, const String& fromString, const String& byString);

    void setContextElement(SVGElement* contextElement) { m_contextElement = contextElement; }
    AnimatedPropertyType type() const { return m_type; }

    bool isAnimatingSVGDom() const { return m_animatedProperty; }
    bool isAnimatingCSSProperty() const { return !m_animatedProperty; }

    void trace(Visitor*);

private:
    SVGAnimatedTypeAnimator(SVGAnimationElement*, SVGElement*);

    friend class ParsePropertyFromString;
    PassRefPtrWillBeRawPtr<SVGPropertyBase> createPropertyForAnimation(const String&);
    PassRefPtrWillBeRawPtr<SVGPropertyBase> resetAnimation(const WillBeHeapVector<RawPtrWillBeMember<SVGElement> >&);

    AnimatedPropertyType m_type;
    RawPtrWillBeMember<SVGAnimationElement> m_animationElement;
    RawPtrWillBeMember<SVGElement> m_contextElement;
    RefPtrWillBeMember<SVGAnimatedPropertyBase> m_animatedProperty;
};

} // namespace blink

#endif // SVGAnimatedTypeAnimator_h
