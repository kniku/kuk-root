import { Component, OnInit, Directive, ViewRef, TemplateRef, ViewContainerRef, ViewChild, ComponentFactoryResolver, ComponentFactory, ComponentRef } from '@angular/core';

@Component({
  selector: 'app-test-alert',
  template: `<p>KURT</p>`,
  // styleUrls: ['./test.component.css']
})
export class AlertComponent implements OnInit {

  constructor() { }

  ngOnInit() {
  }

}

@Component({
  selector: 'app-test',
  templateUrl: './test.component.html',
  styleUrls: ['./test.component.css']
})
export class TestComponent implements OnInit {

  constructor() { }

  ngOnInit() {
  }

}

@Directive({
  selector: '[sidebar]'
})
export class SidebarDirective {
  @ViewChild("alertContainer", { read: ViewContainerRef }) container;
  // private _viewRef : ViewRef;
  componentRef: ComponentRef<AlertComponent>;
  
  constructor(public tpl: TemplateRef<any>, private viewContainer: ViewContainerRef,private resolver: ComponentFactoryResolver ) {}

  ngOnInit() {
    // this._viewRef = this._sidebarService.vcr.createEmbeddedView(this.tpl);
    this.tpl.createEmbeddedView("<p>KURT</p>")
    // this.viewContainer.createEmbeddedView(this.tpl);
  }
  createComponent(type) {
    this.container.clear(); 
    const factory: ComponentFactory<AlertComponent> = this.resolver.resolveComponentFactory(AlertComponent);
    this.componentRef = this.container.createComponent(factory);
  }
  ngOnDestroy() {
    // this._viewRef.destroy();
  }

}
