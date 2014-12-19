#include "pch.h"
#include <iomanip>

namespace NativeScript {
namespace Tests {

TEST_CLASS(EventsTests) {
public:
    TEST_METHOD(Events) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.EventsClass"};
        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(name))};
        IteratorRange<ClassDeclaration::EventIterator> events{declaration->events()};

        ClassDeclaration::EventIterator it{events.begin()};
        EventDeclaration emptyEvent{*it};
        Assert::IsTrue(emptyEvent.name() == L"EmptyEvent");

        ++it;
        EventDeclaration externalEvent{*it};
        Assert::IsTrue(externalEvent.name() == L"ExternalEvent");

        ++it;
        EventDeclaration typedEvent{*it};
        Assert::IsTrue(typedEvent.name() == L"GenericEvent");

        ++it;
        Assert::IsTrue(it == events.end());
    }

    TEST_METHOD(EmptyEvent) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.EventsClass"};
        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(name))};

        const wchar_t* eventName{L"EmptyEvent"};
        EventDeclaration emptyEvent{*dynamic_cast<EventDeclaration*>(declaration->findMembersWithName(eventName)[0].get())};
        Assert::IsTrue(emptyEvent.name() == eventName);
        Assert::IsTrue(emptyEvent.fullName() == eventName);
        Assert::IsTrue(emptyEvent.type().fullName() == L"NativeScript.Tests.Fixtures.EmptyDelegate");
        Assert::IsTrue(emptyEvent.addMethod().fullName() == L"add_EmptyEvent");
        Assert::IsTrue(emptyEvent.removeMethod().fullName() == L"remove_EmptyEvent");
    }

    TEST_METHOD(ExternalEvent) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.EventsClass"};
        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(name))};

        const wchar_t* eventName{L"ExternalEvent"};
        EventDeclaration externalEvent{*dynamic_cast<EventDeclaration*>(declaration->findMembersWithName(eventName)[0].get())};
        Assert::IsTrue(externalEvent.name() == eventName);
        Assert::IsTrue(externalEvent.fullName() == eventName);
        Assert::IsTrue(externalEvent.type().fullName() == L"Windows.UI.Xaml.RoutedEventHandler");
        Assert::IsTrue(externalEvent.addMethod().fullName() == L"add_ExternalEvent");
        Assert::IsTrue(externalEvent.removeMethod().fullName() == L"remove_ExternalEvent");
    }

    TEST_METHOD(ExternalGenericEvent) {
        MetadataReader metadataReader;

        const wchar_t* name{L"NativeScript.Tests.Fixtures.EventsClass"};
        shared_ptr<ClassDeclaration> declaration{dynamic_pointer_cast<ClassDeclaration>(metadataReader.findByName(name))};

        const wchar_t* eventName{L"GenericEvent"};
        EventDeclaration typedEvent{*dynamic_cast<EventDeclaration*>(declaration->findMembersWithName(eventName)[0].get())};
        Assert::IsTrue(typedEvent.name() == eventName);
        Assert::IsTrue(typedEvent.fullName() == eventName);
        Assert::IsTrue(typedEvent.type().fullName() == L"Windows.Foundation.TypedEventHandler`2");
        Assert::IsTrue(typedEvent.addMethod().fullName() == L"add_GenericEvent");
        Assert::IsTrue(typedEvent.removeMethod().fullName() == L"remove_GenericEvent");
    }

    TEST_METHOD(GenericEvent) {
        MetadataReader metadataReader;

        const wchar_t* name{L"Windows.Media.Playback.IMediaPlayer"};
        shared_ptr<InterfaceDeclaration> declaration{dynamic_pointer_cast<InterfaceDeclaration>(metadataReader.findByName(name))};

        const wchar_t* eventName{L"MediaOpened"};
        EventDeclaration typedEvent{*dynamic_cast<EventDeclaration*>(declaration->findMembersWithName(eventName)[0].get())};
        Assert::IsTrue(typedEvent.name() == eventName);
        Assert::IsTrue(typedEvent.fullName() == eventName);
        Assert::IsTrue(typedEvent.type().fullName() == L"Windows.Foundation.TypedEventHandler`2");
        Assert::IsTrue(typedEvent.addMethod().fullName() == L"add_MediaOpened");
        Assert::IsTrue(typedEvent.removeMethod().fullName() == L"remove_MediaOpened");
    }
};

}
}
